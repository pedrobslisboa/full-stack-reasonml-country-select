let route = "/";

[@react.component]
let make = () => {
  let (selectedCountry, setSelectedCountry) = React.useState(() => None);

  let onChange = value => {
    setSelectedCountry(_ => value);
  };

  <Components.Layout>
    <div className="select_wrapper">
      <UniversalPortal_Shared.Portal selector="head">
        <title> {"Country Select" |> React.string} </title>
      </UniversalPortal_Shared.Portal>
      <Components.CountrySelect country=selectedCountry onChange />
    </div>
  </Components.Layout>;
};
