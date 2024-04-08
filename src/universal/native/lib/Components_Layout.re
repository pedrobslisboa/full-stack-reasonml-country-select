[@react.component]
let make = (~children, ~className="") => {
  <div className={Utils.classNames(["layout", className])}>
    <div className="content"> children </div>
  </div>;
};
