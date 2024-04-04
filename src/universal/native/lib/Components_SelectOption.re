type optionRec = {
  label: string,
  value: string,
};

[@react.component]
let make = (~value, ~children, ~selected, ~active, ~onMouseOver, ~onChange) => {
  <li
    className={selected ? "option selected" : "option"}
    key=value
    role="option"
    ariaSelected=active
    onMouseOver
    value>
    <label className="select_list_option">
      <input type_="radio" id=value value checked=selected onChange />
      children
    </label>
  </li>;
};
