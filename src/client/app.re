let rec joinUrlPaths = paths =>
  switch (paths) {
  | [] => "/"
  | [hd] => "/" ++ hd
  | [hd, ...tail] => hd ++ "/" ++ joinUrlPaths(tail)
  };

module Main = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();

    switch (url.path) {
    | x when joinUrlPaths(x) == Shared_js_demo.Pages_CountrySelect.route =>
      <Shared_js_demo.Pages_CountrySelect />
    | _ => <div> {React.string("404")} </div>
    };
  };
};

switch (ReactDOM.querySelector("#root")) {
| Some(el) => <Main /> |> ReactDOM.hydrate(_, el)
| None => ()
};
