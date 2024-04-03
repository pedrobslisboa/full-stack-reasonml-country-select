// Client only, not isomorphic
external fetch: string => Js.Promise.t(Json.t) = "fetch";

module Response = {
  [@mel.send] external json: Json.t => Js.Promise.t(Json.t) = "json";
};
