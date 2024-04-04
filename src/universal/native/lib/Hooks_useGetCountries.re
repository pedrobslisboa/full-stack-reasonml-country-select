type country = {
  label: string,
  value: string,
  population: int,
};

type return = {
  data: list(country),
  error: option(string),
  loading: bool,
};

let decodeCountry: Bindings.Json.t => country =
  json =>
    Bindings.Json.Decode.{
      label: json |> field("label", string),
      value: json |> field("value", string),
      population: json |> field("population", int),
    };

let decodeCountryList: Bindings.Json.t => list(country) =
  json => json |> Bindings.Json.Decode.list(decodeCountry);

let make = () => {
  let result = Hooks_useFetch.make("/api/countries");

  let decodedData =
    React.useMemo1(
      () =>
        switch (result.data) {
        | Some(countries) => decodeCountryList(countries)
        | None => []
        },
      [|result.data|],
    );

  {data: decodedData, error: result.error, loading: result.loading};
};
