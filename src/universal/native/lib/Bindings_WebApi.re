// Client only, not isomorphic
module Element = {
  type t('a) = 'a;

  type scrollIntoViewOptions = {
    block: option(string),
    behavior: option(string),
  };

  [@mel.return nullable]
  external querySelector: string => option(t('a)) = "document.querySelector";
  [@mel.send] external scrollIntoView: (t('a), 'a) => unit = "scrollIntoView";

  [@mel.send] external focus: t('a) => unit = "focus";
  [@mel.send] external blur: t('a) => unit = "blur";
};

module Node = {
  type t('a) = 'a;

  [@mel.send] external contains: (t('a), t('a)) => bool = "contains";
};

module Document = {
  external addEventListener: (string, 'a => unit) => unit =
    "document.addEventListener";
  external removeEventListener: (string, 'a => unit) => unit =
    "document.removeEventListener";
};
