type optionRec = {
  value: string,
  label: string,
};

[@react.component]
let make =
    (
      ~onChange,
      ~value as currentValue=None,
      ~options,
      ~className="",
      ~prefix=React.null,
      ~placeholder="Select a value",
      ~renderOption=?,
    ) => {
  let buttonRef = React.useRef(Bindings.Js.Nullable.null);
  let dropdownRef = React.useRef(Bindings.Js.Nullable.null);

  let (activeIndex, setActiveIndex) = React.useState(() => (-1));
  let (isOpen, setIsOpen) = React.useState(() => false);
  let (searchValue, setSearchValue) = React.useState(() => "");

  let selectedOption =
    switch (currentValue) {
    | Some(value) =>
      List.find_opt((option: optionRec) => option.value == value, options)
    | None => None
    };

  let filteredOptions: list(optionRec) = {
    switch (searchValue) {
    | "" => options
    | _ =>
      List.filter(
        (option: optionRec) => {
          Bindings.Js.String.includes(
            searchValue |> String.trim |> String.lowercase_ascii,
            String.lowercase_ascii(option.label),
          )
        },
        options,
      )
    };
  };

  let handleChange = e => {
    onChange(Some(Bindings.Event.Form.target(e).value));
    setIsOpen(_ => false);
  };

  let handleKeyDown = e => {
    switch (Bindings.Event.Keyboard.key(e)) {
    | "ArrowDown" =>
      if (!isOpen) {
        setIsOpen(_ => true);
      };

      setActiveIndex(prevIndex =>
        prevIndex >= (filteredOptions |> List.length) - 1 ? 0 : prevIndex + 1
      );
    | "ArrowUp" =>
      if (!isOpen) {
        setIsOpen(_ => true);
      };

      setActiveIndex(prevIndex =>
        prevIndex <= 0 ? (filteredOptions |> List.length) - 1 : prevIndex - 1
      );
    | "Esc" => setIsOpen(_ => false)
    | "Backspace" =>
      if (searchValue == "" || !isOpen) {
        onChange(None);
      }
    | "Enter" =>
      switch ((filteredOptions |> Array.of_list)[activeIndex]) {
      | exception (Invalid_argument(_)) => ()
      | option =>
        ReactEvent.Keyboard.preventDefault(e);
        onChange(Some(option.value));
        setIsOpen(_ => false);
      }
    | _ => ()
    };
  };

  let onSearchChange = e => {
    let searchValue = Bindings.Event.Form.target(e).value;

    setActiveIndex(_ => (-1));
    setSearchValue(_ => searchValue);
  };

  let handleToggle = () => {
    if (isOpen) {
      Option.map(
        el => Bindings.WebApi.Element.focus(el),
        buttonRef.current |> Bindings.Js.Nullable.toOption,
      )
      |> ignore;
    };

    setIsOpen(prevOpen => !prevOpen);
  };

  Hooks.UseClickOutsideHandler.make(dropdownRef, _ =>
    if (isOpen) {
      setIsOpen(_ => false);
    }
  );

  React.useEffect1(
    () => {
       if (activeIndex >= 0) {
        let activeElement =
          Bindings.WebApi.Element.querySelector("li[aria-selected=true]");

        switch (activeElement) {
        | None => ()
        | Some(activeElement) =>
          Bindings.WebApi.Element.scrollIntoView(
            activeElement,
            {"block": "nearest", "behavior": "smooth"},
          )
        };
      };

      None;
    },
    [|activeIndex|],
  );

  <div className={Utils.classNames(["select", className])}>
    <button
      ariaHaspopup="listbox"
      ariaControls="dropdown"
      ariaExpanded=isOpen
      value={
        switch (currentValue) {
        | None => ""
        | Some(value) => value
        }
      }
      ariaActivedescendant={
        switch (currentValue) {
        | None => ""
        | Some(value) => value
        }
      }
      onKeyDown=handleKeyDown
      className="select_button"
      onClick={_ => handleToggle()}
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
    <div ref={ReactDOM.Ref.domRef(dropdownRef)} className="select_dropdown">
      <label className="select_search">
        <img className="select_flag" src="/public/magnify-glass.svg" />
        <input
          ariaLabel="Search"
          ref={ReactDOM.Ref.domRef(buttonRef)}
          type_="text"
          name="search"
          placeholder="Search"
          onKeyDown=handleKeyDown
          onChange=onSearchChange
          value=searchValue
        />
      </label>
      <ul
        className="select_list_options"
        id="dropdown"
        role="listbox"
        tabIndex=(-1)>
        {filteredOptions
         |> List.mapi((index, option: optionRec) =>
              <Components_SelectOption
                key={option.value}
                selected={
                  switch (selectedOption) {
                  | Some(selectedOption) =>
                    selectedOption.value == option.value
                  | None => false
                  }
                }
                value={option.value}
                active={activeIndex == index}
                onMouseOver={_ => setActiveIndex(_ => index)}
                onChange=handleChange>
                {switch (renderOption) {
                 | Some(renderOption) => renderOption(option)
                 | None => option.label |> React.string
                 }}
              </Components_SelectOption>
            )
         |> Array.of_list
         |> React.array}
      </ul>
    </div>
  </div>;
};
