open Jest;

[%mel.raw "Element.prototype.scrollIntoView = jest.fn()"];

module Component = {
  [@react.component]
  let make = (~options, ~initialValue=?, ~onChange=?) => {
    let (selectedOption, setSelectedOption) =
      React.useState(() => initialValue);

    <Shared_js_demo.Components.Select
      value=selectedOption
      options
      onChange={value => {
        switch (onChange) {
        | Some(onChange) => onChange(value)
        | None => ()
        };
        setSelectedOption(_ => value);
      }}
    />;
  };
};

describe("Select", () => {
  open ReactTestingLibrary;
  open JestDom;

  let options: list(Shared_js_demo.Components.Select.optionRec) = [
    {value: "1", label: "Option 1"},
    {value: "2", label: "Option 2"},
    {value: "3", label: "Option 3"},
  ];

  test("must render Select", () => {
    let selectComponent = <Component options initialValue="3" />;
    let result = selectComponent |> render;

    result
    |> getByRole(~matcher=`Str("combobox"))
    |> expect
    |> toBeInTheDocument
    |> ignore;

    result
    |> getByRole(~matcher=`Str("combobox"))
    |> expect
    |> toHaveValue(`Str("3"));
  });

  test("must call onChange", () => {
    let value = ref(None);
    let selectComponent =
      <Component options onChange={newValue => value := newValue} />;
    let result = selectComponent |> render;

    getByText(result, ~matcher=`Str("Select a value"))
    |> ReactTestingLibrary.FireEvent.click;

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("radio"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Option 2", ()),
      ),
    );

    value^ |> Expect.expect |> Expect.toBe(Some("2"));
  });

  test("must select a value", () => {
    let selectComponent = <Component options />;

    let result = selectComponent |> render;

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("radio"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Option 2", ()),
      ),
    );

    result
    |> getByRole(~matcher=`Str("combobox"))
    |> expect
    |> toHaveValue(`Str("2"));
  });

  test("must select a value with keyboard", () => {
    let selectComponent = <Component options />;

    let result = selectComponent |> render;

    getByText(result, ~matcher=`Str("Select a value"))
    |> ReactTestingLibrary.FireEvent.click;

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(result, ~matcher=`Str("combobox")),
      ~eventInit={"key": "ArrowDown"},
    );

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(result, ~matcher=`Str("combobox")),
      ~eventInit={"key": "Enter"},
    );

    result
    |> getByRole(~matcher=`Str("combobox"))
    |> expect
    |> toHaveValue(`Str("1"));
  });

  test("must erase a value", () => {
    let selectComponent = <Component options />;

    let result = selectComponent |> render;

    getByText(result, ~matcher=`Str("Select a value"))
    |> ReactTestingLibrary.FireEvent.click;

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("radio"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Option 2", ()),
      ),
    );

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(result, ~matcher=`Str("combobox")),
      ~eventInit={"key": "Backspace"},
    );

    result
    |> getByRole(~matcher=`Str("combobox"))
    |> expect
    |> toHaveValue(`Str(""));
  });

  test("must search a value", () => {
    let selectComponent = <Component options />;

    let result = selectComponent |> render;

    getByText(result, ~matcher=`Str("Select a value"))
    |> ReactTestingLibrary.FireEvent.click;

    ReactTestingLibrary.FireEvent.change(
      getByRole(
        result,
        ~matcher=`Str("textbox"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Search", ()),
      ),
      ~eventInit={
        "target": {
          "value": "Option 2",
        },
      },
    );

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(result, ~matcher=`Str("combobox")),
      ~eventInit={"key": "ArrowDown"},
    );

    ReactTestingLibrary.FireEvent.keyDown(
      getByRole(result, ~matcher=`Str("combobox")),
      ~eventInit={"key": "Enter"},
    );

    result
    |> getByRole(~matcher=`Str("combobox"))
    |> expect
    |> toHaveValue(`Str("2"));
  });
});
