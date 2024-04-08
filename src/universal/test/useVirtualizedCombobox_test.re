open Jest;

[@mel.get] external not_: 'a => 'a = "not";

type optionRec = {
  value: string,
  label: string,
};

module VirtualizedCombobox = {
  type item = optionRec;
};

module UseVirtualizedCombobox =
  Shared_js_demo.Hooks.UseVirtualizedCombobox.Make(VirtualizedCombobox);

let options = [
  {value: "1", label: "Brazil"},
  {value: "2", label: "Argentina"},
  {value: "3", label: "Chile"},
  {value: "4", label: "Uruguay"},
  {value: "5", label: "Paraguay"},
  {value: "6", label: "Bolivia"},
  {value: "7", label: "Peru"},
  {value: "8", label: "Ecuador"},
  {value: "9", label: "Colombia"},
  {value: "10", label: "Venezuela"},
];

module Component = {
  [@react.component]
  let make = (~onChange) => {
    let combobox =
      UseVirtualizedCombobox.make(
        ~options,
        ~onChange=
          (option: option(optionRec)) => {
            switch (option) {
            | Some(value) => onChange(Some(value.value))
            | None => onChange(None)
            }
          },
        ~itemToString=option => option.label,
        ~itemHeight=20,
        ~visibleItems=2,
        (),
      );

    let buttonProps = combobox.getButtonProps();
    let inputProps = combobox.getInputProps();

    <div>
      <button onKeyDown={buttonProps.onKeyDown} onClick={buttonProps.onClick}>
        {React.string("Select")}
      </button>
      <input
        onKeyDown={inputProps.onKeyDown}
        onChange={inputProps.onChange}
        value={inputProps.value}
      />
      {combobox.virtualizedList.visibleItems
       |> List.mapi((index, country) =>
            <button
              key={country.value} onClick={_ => combobox.handleChange(index)}>
              {country.label ++ " " ++ country.value |> React.string}
            </button>
          )
       |> Array.of_list
       |> React.array}
    </div>;
  };
};

describe("useVirtualizedCombobox", () => {
  open ReactTestingLibrary;
  open JestDom;

  test("must return list", () => {
    let selectComponent = <Component onChange={_ => ()} />;
    let result = selectComponent |> render;

    result
    |> getByRole(
         ~matcher=`Str("button"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Brazil 1", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    result
    |> getByRole(
         ~matcher=`Str("button"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
       )
    |> expect
    |> toBeInTheDocument;
  });

  testPromise("must navigate down", () => {
    let selectComponent = <Component onChange={_ => ()} />;
    let result = selectComponent |> render;

    result
    |> getByRole(
         ~matcher=`Str("button"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Brazil 1", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    result
    |> getByRole(
         ~matcher=`Str("button"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
      ),
      ~eventInit={"key": "ArrowDown"},
    );

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
      ),
      ~eventInit={"key": "ArrowDown"},
    );

    result
    |> getByRole(
         ~matcher=`Str("button"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    let callback = () => {
      result
      |> getByRole(
           ~matcher=`Str("button"),
           ~options=
             DomTestingLibrary.ByRoleQuery.makeOptions(~name="Chile 3", ()),
         )
      |> expect
      |> toBeInTheDocument
      |> ignore;

      ();
    };

    DomTestingLibrary.waitFor(~callback, ())
    |> Js.Promise.then_(_ => {Js.Promise.resolve(pass)});
  });

  testPromise("must navigate up", () => {
    let selectComponent = <Component onChange={_ => ()} />;
    let result = selectComponent |> render;

    result
    |> getByRole(
         ~matcher=`Str("button"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Brazil 1", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    result
    |> getByRole(
         ~matcher=`Str("button"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
      ),
      ~eventInit={"key": "ArrowDown"},
    );

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
      ),
      ~eventInit={"key": "ArrowDown"},
    );

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Argentina 2", ()),
      ),
      ~eventInit={"key": "ArrowUp"},
    );

    let callback = () => {
      result
      |> getByRole(
           ~matcher=`Str("button"),
           ~options=
             DomTestingLibrary.ByRoleQuery.makeOptions(
               ~name="Argentina 2",
               (),
             ),
         )
      |> expect
      |> toBeInTheDocument
      |> ignore;

      result
      |> getByRole(
           ~matcher=`Str("button"),
           ~options=
             DomTestingLibrary.ByRoleQuery.makeOptions(~name="Brazil 1", ()),
         )
      |> expect
      |> toBeInTheDocument
      |> ignore;
      ();
    };

    DomTestingLibrary.waitFor(~callback, ())
    |> Js.Promise.then_(_ => {Js.Promise.resolve(pass)});
  });

  testPromise("must select", () => {
    let value = ref(None);
    let selectComponent = <Component onChange={option => value := option} />;
    let result = selectComponent |> render;

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Brazil 1", ()),
      ),
    );

    let callback = () => {
      Expect.expect(value^) |> Expect.toEqual(Some("1")) |> ignore;

      ();
    };

    DomTestingLibrary.waitFor(~callback, ())
    |> Js.Promise.then_(_ => {Js.Promise.resolve(pass)});
  });
});
