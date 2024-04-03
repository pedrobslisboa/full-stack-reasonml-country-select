let route = "/";

[@react.component]
let make = () => {
  let (selectedCountry, setSelectedCountry) = React.useState(() => None);

  setSelectedCountry |> ignore;
  let%browser_only onChange = _value => {
    setSelectedCountry(_ => _value);
  };

  <Components.Layout>
    <div className="select_wrapper">
      <UniversalPortal_Shared.Portal selector="head">
        <title> {"Country Select" |> React.string} </title>
      </UniversalPortal_Shared.Portal>
      <Components.CountrySelect value=selectedCountry onChange />
    </div>
  </Components.Layout>;
};
