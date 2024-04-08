module CountryFlag = {
  [@react.component]
  let make = (~country) => {
    <span
      className={Utils.classNames(["fi", "fi-" ++ country, "country_flag"])}
    />;
  };
};

[@react.component]
let make = (~onChange, ~country, ~className="") => {
  let {data, loading, error}: Hooks.UseGetCountries.return =
    Hooks.UseGetCountries.make();

  let options =
    data
    |> List.map(({label, value, _}: Hooks.UseGetCountries.country) => {
         let countryOption: Components_Select.optionRec = {label, value};

         countryOption;
       });

  let handleChange = value => {
    onChange(value);
  };

  switch (error) {
  | Some(_) => <p> {"Ops someting went wrong" |> React.string} </p>
  | None =>
    !loading
      ? <Components_Select
          className
          itemHeight=28
          visibleItems=10
          dropdownClassName="country_dropdown"
          prefix={
            switch (country) {
            | Some(v) => <CountryFlag country=v />
            | None => React.null
            }
          }
          value=country
          placeholder="Select a country"
          onChange=handleChange
          options
          renderOption={({value, label}: Components_Select.optionRec) =>
            <>
              <CountryFlag country=value />
              <span className="country_name"> {label |> React.string} </span>
              <span className="country_population">
                {Utils.formatPopulation(
                   data
                   |> List.find((country: Hooks.UseGetCountries.country) =>
                        country.value == value
                      )
                   |> (x => x.population |> float_of_int),
                 )
                 |> React.string}
              </span>
            </>
          }
        />
      : React.null
  };
};
