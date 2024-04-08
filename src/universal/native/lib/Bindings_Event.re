module Form = {
  type t = ReactEvent.Form.t;
  type target = {value: string};

  [@mel.get] external target: t => Bindings_WebApi.Element.t('a) = "target";
};

module Keyboard = {
  type t = ReactEvent.Keyboard.t;

  [@mel.get] external key: t => string = "key";
};

module UI = {
  type t = ReactEvent.UI.t;

  [@mel.get] external target: t => Bindings_WebApi.Element.t('a) = "target";
};

module Mouse = {
  type t = ReactEvent.Mouse.t;
};
