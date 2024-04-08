type optionRec = {
  value: string,
  label: string,
};

module Combobox = {
  type item = optionRec;
};

module Spread = {
  [@react.component]
  let make = (~props, ~children) => React.cloneElement(children, props);
};
module UseVirtualizedCombobox = Hooks.UseVirtualizedCombobox.Make(Combobox);

[@react.component]
let make =
    (
      ~onChange,
      ~value as currentValue=None,
      ~options,
      ~className="",
      ~dropdownClassName="",
      ~listClassName="",
      ~prefix=React.null,
      ~placeholder="Select a value",
      ~renderOption=?,
      ~visibleItems=10,
      ~itemHeight=28,
    ) => {
  let combobox =
    UseVirtualizedCombobox.make(
      ~options,
      ~onChange=
        (option: option(optionRec)) => {
          switch (option) {
          | Some(value) => onChange(Some(value.value))
          | None => onChange(None)
          }
        },
      ~itemToString=option => option.label,
      ~itemHeight,
      ~visibleItems,
      (),
    );

  let selectedOption =
    switch (currentValue) {
    | Some(value) =>
      List.find_opt((option: optionRec) => option.value == value, options)
    | None => None
    };

  let stringValue =
    switch (currentValue) {
    | Some(value) => value
    | None => ""
    };

  let buttonProps = combobox.getButtonProps();
  let inputProps = combobox.getInputProps();
  let dropdownProps = combobox.getDropdownProps();

  let virtualizedContainerProps =
    combobox.virtualizedList.getVirtualizedContainerProps();
  let virtualizedListWrapperProps =
    combobox.virtualizedList.getVirtualizedListWrapperProps();
  let virtualizedListProps =
    combobox.virtualizedList.getVirtualizedListProps();

  <div className={Utils.classNames(["select", className])}>
    <button
      ariaHaspopup="listbox"
      ariaControls="dropdown"
      ariaExpanded={combobox.isOpen}
      value=stringValue
      ariaActivedescendant=stringValue
      onKeyDown={buttonProps.onKeyDown}
      className="select_button"
      onClick={buttonProps.onClick}
      role="combobox">
      prefix
      <span className="select_label">
        {React.string(
           switch (selectedOption) {
           | Some(option) => option.label
           | _ => placeholder
           },
         )}
      </span>
      <img className="caret-down" src="/public/caret-down.svg" />
    </button>
    <div
      ref={ReactDOM.Ref.domRef(dropdownProps.ref)}
      className={Utils.classNames(["select_dropdown", dropdownClassName])}>
      <label className="select_search">
        <img className="select_flag" src="/public/magnify-glass.svg" />
        <input
          ariaLabel="Search"
          type_="text"
          name="search"
          placeholder="Search"
          onKeyDown={inputProps.onKeyDown}
          ref={ReactDOM.Ref.domRef(inputProps.ref)}
          onChange={inputProps.onChange}
          value={inputProps.value}
        />
      </label>
      <div
        style={virtualizedContainerProps.style}
        onScroll={virtualizedContainerProps.onScroll}
        ref={virtualizedContainerProps.ref}>
        <div style={virtualizedListWrapperProps.style}>
          <ul
            style={virtualizedListProps.style}
            className={Utils.classNames([
              "select_list_options",
              listClassName,
            ])}
            id="dropdown"
            role="listbox"
            tabIndex=(-1)>
            {combobox.virtualizedList.visibleItems
             |> List.mapi((index, option: optionRec) => {
                  let virtualizedOptionProps =
                    combobox.virtualizedList.getVirtualizedOptionProps(index);

                  <Components_SelectOption
                    style={ReactDOM.Style.make(
                      ~height=(itemHeight |> string_of_int) ++ "px",
                      (),
                    )}
                    key={option.value}
                    selected={
                      switch (selectedOption) {
                      | Some(selectedOption) =>
                        selectedOption.value == option.value
                      | None => false
                      }
                    }
                    value={option.value}
                    active={virtualizedOptionProps.ariaSelected}
                    onClick={_ => combobox.close()}
                    onMouseOver={virtualizedOptionProps.onMouseOver}
                    onChange={_ => combobox.handleChange(index)}>
                    {switch (renderOption) {
                     | Some(renderOption) => renderOption(option)
                     | None => option.label |> React.string
                     }}
                  </Components_SelectOption>;
                })
             |> Array.of_list
             |> React.array}
          </ul>
        </div>
      </div>
    </div>
  </div>;
};
