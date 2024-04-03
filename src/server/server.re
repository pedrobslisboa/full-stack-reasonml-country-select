open Lwt;

let indexFile = open_in_bin("./public/index.html");
let indexFileString =
  really_input_string(indexFile, in_channel_length(indexFile));

let handler =
  Dream.router([
    Dream.get("/public/**", Dream.static("./public")),
    Dream.get("/api/countries", _ => {
      Api_Countries.getCountries()
      >>= (
        countries =>
          switch (countries) {
          | None => Dream.json("null")
          | Some(countriesValues) =>
            Yojson.Basic.to_string(countriesValues) |> Dream.json
          }
      )
    }),
    Dream.get("/", _ => {
      let portals: ref(array(UniversalPortal_Shared.portal)) = ref([||]);

      let html =
        ReactDOM.renderToString(
          UniversalPortal_Server.collectPortals(
            <Shared_native_demo.Pages_CountrySelect />,
            (collectedPortal: UniversalPortal_Shared.portal) => {
            portals := Array.append(portals^, [|collectedPortal|])
          }),
        );

      Str.global_replace(
        Str.regexp("<div id=\"root\"></div>"),
        "<div id=\"root\">" ++ html ++ "</div>",
        indexFileString,
      )
      |> UniversalPortal_Server.appendUniversalPortals(_, portals^)
      |> Dream.html;
    }),
    Dream.get("**", _ => Dream.html(indexFileString)),
  ]);

let interface =
  switch (Sys.getenv_opt("SERVER_INTERFACE")) {
  | Some(env) => env
  | None => "localhost"
  };

Dream.run(~port=8000, ~interface, handler);
