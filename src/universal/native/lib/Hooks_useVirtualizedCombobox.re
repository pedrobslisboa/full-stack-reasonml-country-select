module type S = {
  type item;
};

type buttonProps = {
  onClick: ReactEvent.Mouse.t => unit,
  onKeyDown: ReactEvent.Keyboard.t => unit,
  ref: React.ref(Bindings.Js.Nullable.t(Dom.element)),
};
type inputProps = {
  onChange: ReactEvent.Form.t => unit,
  onKeyDown: ReactEvent.Keyboard.t => unit,
  value: string,
  ref: React.ref(Bindings.Js.Nullable.t(Dom.element)),
};
type dropdownProps = {ref: React.ref(Bindings.Js.Nullable.t(Dom.element))};

module Make = (M: S) => {
  module VirtualizedList = {
    type item = M.item;
  };

  module UseVirtualizedList = Hooks_useVirtualizedList.Make(VirtualizedList);

  type return = {
    isOpen: bool,
    items: list(M.item),
    virtualizedList: UseVirtualizedList.return,
    getButtonProps: unit => buttonProps,
    getInputProps: unit => inputProps,
    getDropdownProps: unit => dropdownProps,
    handleChange: int => unit,
    handleToggle: unit => unit,
    handleSearchChange: ReactEvent.Form.t => unit,
    close: unit => unit,
  };

  let make =
      (
        ~options: list(M.item),
        ~onChange,
        ~itemToString,
        ~itemHeight=28,
        ~visibleItems=10,
        (),
      ) => {
    let buttonRef = React.useRef(Bindings.Js.Nullable.null);
    let dropdownRef = React.useRef(Bindings.Js.Nullable.null);
    let inputRef = React.useRef(Bindings.Js.Nullable.null);

    let (isOpen, setIsOpen) = React.useState(() => false);
    let (searchValue, setSearchValue) = React.useState(() => "");

    let filteredOptions: list('a) = {
      switch (searchValue) {
      | "" => options
      | _ =>
        List.filter(
          option => {
            Bindings.Js.String.includes(
              searchValue |> String.trim |> String.lowercase_ascii,
              String.lowercase_ascii(itemToString(option)),
            )
          },
          options,
        )
      };
    };

    let virtualizedList =
      UseVirtualizedList.make(
        ~items=filteredOptions,
        ~itemHeight=itemHeight |> float_of_int,
        ~containerHeight=visibleItems * itemHeight |> float_of_int,
        ~onSelect=onChange,
        (),
      );

        let handleToggle = () => {
      if (!isOpen) {
        Option.map(
          el => Bindings.WebApi.Element.focus(el),
          inputRef.current |> Bindings.Js.Nullable.toOption,
        )
        |> ignore;
      };

      setIsOpen(prevOpen => {
        if (prevOpen) {
          Option.map(
            el => Bindings.WebApi.Element.focus(el),
            inputRef.current |> Bindings.Js.Nullable.toOption,
          )
          |> ignore;
        };

        !prevOpen;
      });
    };


    let handleKeyDown = e => {
      switch (Bindings.Event.Keyboard.key(e)) {
      | "ArrowDown" =>
        ReactEvent.Keyboard.preventDefault(e);

        switch (isOpen) {
        | false => setIsOpen(_ => true)
        | true => virtualizedList.navigate(Down)
        };

      | "ArrowUp" =>
        ReactEvent.Keyboard.preventDefault(e);

        switch (isOpen) {
        | false => setIsOpen(_ => true)
        | true =>
          setIsOpen(_ => true);
          virtualizedList.navigate(Up);
        };
      | "Escape" => setIsOpen(_ => false)
      | "Backspace" =>
        if (searchValue == "" || !isOpen) {
          virtualizedList.navigate(Clear);
          setSearchValue(_ => "");
          onChange(None);
        }
      | "Enter" =>
        ReactEvent.Keyboard.preventDefault(e);
        handleToggle();

        if(isOpen) {
          virtualizedList.navigate(Select);
        }
      | _ => ()
      };
    };

    let handleSearchChange = e => {
      virtualizedList.navigate(Clear);
      let searchValue = Bindings.Event.Form.target(e)##value;

      setSearchValue(_ => searchValue);
    };

    let handleChange = index => {
      let option =
        switch (List.nth_opt(virtualizedList.visibleItems, index)) {
        | exception (Invalid_argument(_)) => None
        | option => option
        };

      onChange(option);
      setIsOpen(_ => false);
    };

    Hooks_useClickOutsideHandler.make(dropdownRef, _ =>
      if (isOpen) {
        setIsOpen(_ => false);
      }
    );

    let getButtonProps = () => {
      onClick: _ => handleToggle(),
      onKeyDown: handleKeyDown,
      ref: buttonRef,
    };
    let getInputProps = () => {
      onChange: handleSearchChange,
      onKeyDown: handleKeyDown,
      value: searchValue,
      ref: inputRef,
    };
    let getDropdownProps = () => {ref: dropdownRef};
    let close = () => setIsOpen(_ => false);

    {
      isOpen,
      items: filteredOptions,
      virtualizedList,
      getButtonProps,
      getInputProps,
      getDropdownProps,
      handleChange,
      handleToggle,
      handleSearchChange,
      close,
    };
  };
};
