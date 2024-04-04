open Jest;

[%mel.raw "Element.prototype.scrollIntoView = jest.fn()"];
[%mel.raw
  "
global.fetch = jest.fn(() =>
  Promise.resolve({
    json: () => Promise.resolve(
    {value: 'Brazil'}),
  })
)
"
];

type request = {value: string};

let decodeReq: Shared_js_demo.Bindings.Json.t => request =
  json =>
    Shared_js_demo.Bindings.Json.Decode.{
      value: json |> field("value", string),
    };

module Component = {
  [@react.component]
  let make = () => {
    let result = Shared_js_demo.Hooks.UseFetch.make("SomeUrl");

    let decodeData =
      switch (result.data) {
      | Some(data) => data |> decodeReq
      | None => {value: "No data"}
      };

    <div>
      <h1> {React.string("Data " ++ decodeData.value)} </h1>
      <h1>
        {React.string("Loading ")}
        {result.loading
           ? {
             React.string("true");
           }
           : {
             React.string("false");
           }}
      </h1>
      <h1>
        {React.string("Error ")}
        {switch (result.error) {
         | Some(_) => React.string("true")
         | None => React.string("false")
         }}
      </h1>
    </div>;
  };
};

describe("Select", () => {
  ReactTestingLibrary.(
    JestDom.(
      testPromise("must render Select", () => {
        let selectComponent = <Component />;
        let result = selectComponent |> render;

        result
        |> getByRole(
             ~matcher=`Str("heading"),
             ~options=
               DomTestingLibrary.ByRoleQuery.makeOptions(
                 ~name="Data No data",
                 (),
               ),
           )
        |> expect
        |> toBeInTheDocument
        |> ignore;

        result
        |> getByRole(
             ~matcher=`Str("heading"),
             ~options=
               DomTestingLibrary.ByRoleQuery.makeOptions(
                 ~name="Loading true",
                 (),
               ),
           )
        |> expect
        |> toBeInTheDocument
        |> ignore;

        result
        |> getByRole(
             ~matcher=`Str("heading"),
             ~options=
               DomTestingLibrary.ByRoleQuery.makeOptions(
                 ~name="Error false",
                 (),
               ),
           )
        |> expect
        |> toBeInTheDocument
        |> ignore;

        let callback = () => {
          result
          |> getByRole(
               ~matcher=`Str("heading"),
               ~options=
                 DomTestingLibrary.ByRoleQuery.makeOptions(
                   ~name="Data Brazil",
                   (),
                 ),
             )
          |> expect
          |> toBeInTheDocument
          |> ignore;

          result
          |> getByRole(
               ~matcher=`Str("heading"),
               ~options=
                 DomTestingLibrary.ByRoleQuery.makeOptions(
                   ~name="Loading false",
                   (),
                 ),
             )
          |> expect
          |> toBeInTheDocument
          |> ignore;

          result
          |> getByRole(
               ~matcher=`Str("heading"),
               ~options=
                 DomTestingLibrary.ByRoleQuery.makeOptions(
                   ~name="Error false",
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
