open Jest;

[@mel.send] external not_: JestDom.expect => JestDom.expect = "not";

[%mel.raw "Element.prototype.scrollIntoView = jest.fn()"];
[%mel.raw
  "
global.fetch = jest.fn(() =>
  Promise.resolve({
    json: () => Promise.resolve(
    [{value: 'br', label: 'Brazil', population: 12345}, {value: 'ar', label: 'Argentina', population: 54321}]),
  })
)
"
];

module Component = {
  [@react.component]
  let make = () => {
    let result = Shared_js_demo.Hooks.UseGetCountries.make();

    List.map(
      (country: Shared_js_demo.Hooks.UseGetCountries.country) =>
        <div key={country.value}>
          <h1>
            {country.label
             ++ " "
             ++ (country.population |> string_of_int)
             |> React.string}
          </h1>
        </div>,
      result.data,
    )
    |> Array.of_list
    |> React.array;
  };
};

describe("Select", () => {
  ReactTestingLibrary.(
    JestDom.(
      testPromise("must return decoded country data", () => {
        let selectComponent = <Component />;
        let result = selectComponent |> render;

        let callback = () => {
          result
          |> getByRole(
               ~matcher=`Str("heading"),
               ~options=
                 DomTestingLibrary.ByRoleQuery.makeOptions(
                   ~name="Brazil 12345",
                   (),
                 ),
             )
          |> expect
          |> toBeInTheDocument
          |> ignore;

          ();
        };

        DomTestingLibrary.waitFor(~callback, ())
        |> Js.Promise.then_(_ => {Js.Promise.resolve(pass)});
      })
    )
  )
});
