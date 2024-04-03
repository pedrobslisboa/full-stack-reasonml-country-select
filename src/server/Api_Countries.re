open Lwt;
open Cohttp_lwt_unix;
open Yojson.Basic.Util;

type country = {
  label: string,
  value: string,
  population: int,
};

type countries = {countries: option(list(country))};

let label = json => json |> member("label") |> to_string;
let value = json => json |> member("value") |> to_string;
let population = json => json |> member("population") |> to_int;

let countries = json => {
  `List(
    {
      json
      |> to_list
      |> List.map(country => {
           let value = country |> member("value") |> to_string;
           let label = country |> member("label") |> to_string;

           `Assoc([
             ("value", `String(value)),
             ("label", `String(label)),
             ("population", `Int(population(country))),
           ]);
         });
    },
  );
};

let extractCountries = (json: Yojson.Basic.t) => {
  json
  |> to_list
  |> List.map(country => {
       let value = {
         label: country |> member("label") |> to_string,
         value: country |> member("value") |> to_string,
         population: country |> member("population") |> to_int,
       };

       value;
     });
};

let resultJson = json => json |> countries;

let getCountries = _ => {
  let uri =
    Uri.of_string(
      "https://gist.githubusercontent.com/pedrobslisboa/1a6af944bc5f70067018df6f0ab30263/raw/f987794a5be8bb40bb92595f615024a59123a6dc/country-list-population.json",
    );
  Client.get(uri)
  >>= (
    ((_, body)) => {
      body
      |> Cohttp_lwt.Body.to_string
      >|= (
        body => {
          let json = Yojson.Basic.from_string(body);

          Some(resultJson(json));
        }
      );
    }
  );
};
