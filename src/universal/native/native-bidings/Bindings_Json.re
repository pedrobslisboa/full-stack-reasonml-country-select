open Yojson.Basic.Util;

type t = Yojson.Basic.t;

module Decode = {
  let field = (fieldName, decoder, json) => {
    json |> member(fieldName) |> decoder;
  };

  let string = value => {
    value |> to_string;
  };

  let int = value => {
    value |> to_int;
  };

  let list = (decodeFn, value) => {
    value |> to_list |> List.map(decodeFn);
  };
};

module Utils = {
  let from_string = Yojson.Basic.from_string;
};
