open Jest;

[%mel.raw "Element.prototype.scrollIntoView = jest.fn()"];
[%mel.raw
  "
global.fetch = jest.fn(() =>
  Promise.resolve({
    json: () => Promise.resolve( [
    {value: 'ar', label: 'Argentina', population: 1234},
    {value: 'br', label: 'Brazil', population: 54321},
    {value: 'cl', label: 'Chile', population: 11111},
  ]),
  })
)
"
];

module Component = {
  [@react.component]
  let make = (~initialValue=?, ~onChange=?) => {
    let (selectedOption, setSelectedOption) =
      React.useState(() => initialValue);

    <Shared_js_demo.Components.CountrySelect
      value=selectedOption
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

  testPromise("must render Select", () => {
    let selectComponent = <Component initialValue="3" />;
    let result = selectComponent |> render;

    let callback = () => {
      let _ =
        result
        |> getByRole(~matcher=`Str("combobox"))
        |> expect
        |> toBeInTheDocument
        |> ignore;

      result
      |> getByRole(~matcher=`Str("combobox"))
      |> expect
      |> toHaveValue(`Str("3"))
      |> ignore;
      ();
    };

    DomTestingLibrary.waitFor(~callback, ())
    |> Js.Promise.then_(_ => Js.Promise.resolve(pass));
  });

  testPromise("must change value", () => {
    let selectComponent = <Component />;
    let result = selectComponent |> render;

    let callback = () => {
      result
      |> getByRole(~matcher=`Str("combobox"))
      |> expect
      |> toBeInTheDocument
      |> ignore;
      ();
    };

    DomTestingLibrary.waitFor(~callback, ())
    |> Js.Promise.then_(_ => {
         ReactTestingLibrary.FireEvent.click(
           getByRole(
             result,
             ~matcher=`Str("radio"),
             ~options=
               DomTestingLibrary.ByRoleQuery.makeOptions(
                 ~name="Brazil 54.3k",
                 (),
               ),
           ),
         );

         Js.Promise.resolve(pass);
       });
  });
});
