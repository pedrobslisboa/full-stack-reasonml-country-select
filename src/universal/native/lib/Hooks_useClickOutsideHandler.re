let make =
    (
      reference:
        React.ref(Bindings.Js.Nullable.t(Bindings.WebApi.Element.t('a))),
      handler,
    ) =>
  React.useEffect2(
    () => {
      let listener = e => {
        switch (reference.current |> Bindings.Js.Nullable.toOption) {
        | None => ()
        | Some(refValue) =>
          if (!
                Bindings.WebApi.Node.contains(
                  refValue,
                  Bindings.Event.Form.target(e),
                )) {
            handler(e);
          }
        };
      };

      Bindings.WebApi.Document.addEventListener("click", listener);

      Some(
        () => {
          Bindings.WebApi.Document.removeEventListener("click", listener)
        },
      );
    },
    (reference, handler),
  );
