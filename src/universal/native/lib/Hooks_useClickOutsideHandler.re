let make =
    (
      reference: React.ref(Js.Nullable.t(Bindings.WebApi.Element.t('a))),
      handler,
    ) =>
  React.useEffect2(
    () => {
      let listener = _e => {
        switch (reference.current |> Js.Nullable.toOption) {
        | None => ()
        | Some(refValue) =>
          if (!
                Bindings.WebApi.Element.contains(
                  refValue,
                  Bindings.Event.Form.target(_e),
                )) {
            handler(_e);
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
