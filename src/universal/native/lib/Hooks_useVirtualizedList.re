type navigate =
  | Up
  | Down
  | Clear
  | Select;

module type S = {
  type item;
};

type virtualizedContainerProps = {
  style: ReactDOM.Style.t,
  ref: ReactDOM.domRef,
  onScroll: Bindings.Event.UI.t => unit,
};

type virtualizedListWrapperProps = {style: ReactDOM.Style.t};
type virtualizedListProps = {style: ReactDOM.Style.t};
type virtualizedOptionProps = {
  style: ReactDOM.Style.t,
  onMouseOver: Bindings.Event.Mouse.t => unit,
  ariaSelected: bool,
};

let setElementScrollTop = (element, scrollTop) => {
  let listElement = element;

  Option.map(
    el => Bindings.WebApi.Element.setScrollTop(el, scrollTop) |> ignore,
    listElement,
  )
  |> ignore;
};

module Make = (M: S) => {
  type return = {
    visibleItems: list(M.item),
    getVirtualizedContainerProps: unit => virtualizedContainerProps,
    getVirtualizedListWrapperProps: unit => virtualizedListWrapperProps,
    getVirtualizedListProps: unit => virtualizedListProps,
    getVirtualizedOptionProps: int => virtualizedOptionProps,
    navigate: navigate => unit,
    activeIndex: int,
  };

  let make = (~items, ~containerHeight, ~itemHeight, ~onSelect=_ => (), ()) => {
    let preventScroll = React.useRef(false);
    let virtualizedContainer = React.useRef(Bindings.Js.Nullable.null);
    let (activeIndex, setActiveIndex) = React.useState(() => (-1));
    let (scrollTop, setScrollTop) = React.useState(_ => 0.);
    let startIndex = Bindings.Float.floor(scrollTop /. itemHeight);
    let endIndex =
      Bindings.Float.min(
        startIndex +. Bindings.Float.ceil(containerHeight /. itemHeight),
        List.length(items) |> float_of_int,
      );

    let visibleItems =
      items
      |> Utils.List.slice(
           startIndex |> int_of_float,
           endIndex |> int_of_float,
         );

    let handleScroll = event =>
      if (preventScroll.current) {
        preventScroll.current = false;
      } else {
        setScrollTop(_ => Bindings.Event.UI.target(event)##scrollTop);
      };

    let handleScrollTop = index => {
      preventScroll.current = true;
      let floatIndex = index |> float_of_int;

      setScrollTop(prev => {
        let newScrollTop =
          switch (startIndex, floatIndex, endIndex -. 1.) {
          | (0., x, _) when x <= 0. => 0.
          | (_, x, y)
              when
                y
                +. 1. == (List.length(items) |> float_of_int)
                && x
                +. 1. >= endIndex
                -. startIndex => prev
          | (_, x, _) when x == 0. => startIndex *. itemHeight
          | (_, x, _) when x < 0. => prev -. itemHeight
          | (_, x, _) when x == endIndex -. startIndex => prev +. itemHeight
          | (_, x, _) when x > endIndex -. startIndex => prev +. itemHeight
          | _ => prev
          };

        setElementScrollTop(
          virtualizedContainer.current |> Bindings.Js.Nullable.toOption,
          newScrollTop,
        );

        newScrollTop;
      });
    };

    let navigate = action => {
      switch (action) {
      | Down =>
        setActiveIndex(prevIndex =>
          switch (prevIndex >= (endIndex -. startIndex -. 1. |> int_of_float)) {
          | false =>
            let nextIndex = prevIndex + 1;

            handleScrollTop(nextIndex);

            nextIndex;
          | true =>
            handleScrollTop(prevIndex + 1);

            prevIndex;
          }
        )
      | Up =>
        setActiveIndex(prevIndex =>
          switch (prevIndex <= 0) {
          | false =>
            let nextIndex = prevIndex - 1;

            handleScrollTop(nextIndex);

            nextIndex;
          | true =>
            handleScrollTop(prevIndex - 1);

            prevIndex;
          }
        )
      | Clear =>
        setActiveIndex(_ => {
          setScrollTop(_ => 0.);

          setElementScrollTop(
            virtualizedContainer.current |> Bindings.Js.Nullable.toOption,
            0.,
          );

          (-1);
        })
      | Select =>
        switch (List.nth_opt(visibleItems, activeIndex)) {
        | exception (Invalid_argument(_)) => ()
        | option => onSelect(option)
        }
      };
    };

    let getVirtualizedContainerProps = () => {
      style:
        ReactDOM.Style.make(
          ~position="relative",
          ~height=
            (
              (endIndex -. startIndex)
              *. itemHeight
              |> int_of_float
              |> string_of_int
            )
            ++ "px",
          ~overflowY="auto",
          (),
        ),

      ref: ReactDOM.Ref.domRef(virtualizedContainer),
      onScroll: handleScroll,
    };

    let getVirtualizedListWrapperProps = (): virtualizedListWrapperProps => {
      style:
        ReactDOM.Style.make(
          ~position="relative",
          ~height=
            (
              (items |> List.length)
              * (itemHeight |> int_of_float)
              |> string_of_int
            )
            ++ "px",
          (),
        ),
    };

    let getVirtualizedListProps = (): virtualizedListProps => {
      style:
        ReactDOM.Style.make(
          ~position="absolute",
          ~top=
            (startIndex *. itemHeight |> int_of_float |> string_of_int) ++ "px",
          (),
        ),
    };

    let getVirtualizedOptionProps = index => {
      style:
        ReactDOM.Style.make(
          ~height=(itemHeight |> string_of_float) ++ "px",
          (),
        ),
      onMouseOver: _ => setActiveIndex(_ => index),
      ariaSelected: activeIndex === index,
    };

    {
      visibleItems,
      navigate,
      activeIndex,
      getVirtualizedListProps,
      getVirtualizedContainerProps,
      getVirtualizedListWrapperProps,
      getVirtualizedOptionProps,
    };
  };
};
