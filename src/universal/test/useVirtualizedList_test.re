open Jest;

[@mel.get] external not_: 'a => 'a = "not";

type optionRec = {
  value: string,
  label: string,
};

module VirtualizedList = {
  type item = optionRec;
};

module UseVirtualizedList =
  Shared_js_demo.Hooks.UseVirtualizedList.Make(VirtualizedList);

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
  let make = (~onSelect) => {
    let virtualizedList =
      UseVirtualizedList.make(
        ~items=options,
        ~itemHeight=20.,
        ~containerHeight=20.,
        ~onSelect,
        (),
      );

    <div>
      <button onClick={_ => virtualizedList.navigate(Down)}>
        {React.string("Navigate Down")}
      </button>
      <button onClick={_ => virtualizedList.navigate(Up)}>
        {React.string("Navigate Up")}
      </button>
      <h1>
        {React.string(
           "Length "
           ++ (virtualizedList.visibleItems |> List.length |> string_of_int),
         )}
      </h1>
      {virtualizedList.visibleItems
       |> List.map(country =>
            <button
              key={country.value}
              onClick={_ => virtualizedList.navigate(Select)}>
              {country.label ++ " " ++ country.value |> React.string}
            </button>
          )
       |> Array.of_list
       |> React.array}
    </div>;
  };
};

describe("useVirtualizedList", () => {
  open ReactTestingLibrary;
  open JestDom;

  test("must return list", () => {
    let selectComponent = <Component onSelect={_ => ()} />;
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

    result
    |> getByRole(
         ~matcher=`Str("heading"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Length 2", ()),
       )
    |> expect
    |> toBeInTheDocument;
  });

  testPromise("must navigate down", () => {
    let selectComponent = <Component onSelect={_ => ()} />;
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

    result
    |> getByRole(
         ~matcher=`Str("heading"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Length 2", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(
            ~name="Navigate Down",
            (),
          ),
      ),
    );

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(
            ~name="Navigate Down",
            (),
          ),
      ),
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

      result
      |> getByRole(
           ~matcher=`Str("heading"),
           ~options=
             DomTestingLibrary.ByRoleQuery.makeOptions(~name="Length 2", ()),
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
    let selectComponent = <Component onSelect={_ => ()} />;
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

    result
    |> getByRole(
         ~matcher=`Str("heading"),
         ~options=
           DomTestingLibrary.ByRoleQuery.makeOptions(~name="Length 2", ()),
       )
    |> expect
    |> toBeInTheDocument
    |> ignore;

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(
            ~name="Navigate Down",
            (),
          ),
      ),
    );

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(
            ~name="Navigate Down",
            (),
          ),
      ),
    );

    ReactTestingLibrary.FireEvent.click(
      getByRole(
        result,
        ~matcher=`Str("button"),
        ~options=
          DomTestingLibrary.ByRoleQuery.makeOptions(~name="Navigate Up", ()),
      ),
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
    let selectComponent = <Component onSelect={option => value := option} />;
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
      Expect.expect(value^)
      |> Expect.toEqual(Some({value: "1", label: "Brazil"}))
      |> ignore;

      ();
    };

    DomTestingLibrary.waitFor(~callback, ())
    |> Js.Promise.then_(_ => {Js.Promise.resolve(pass)});
  });
});
