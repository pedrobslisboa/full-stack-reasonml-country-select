let getUnit = value =>
  switch (value) {
  | x when x >= 1000000. => `Millions
  | x when x >= 1000. => `Thousands
  | _ => `Hundreds
  };

let formatPopulation = population => {
  switch (getUnit(population)) {
  | `Millions => population /. 1000000. |> Printf.sprintf("%.1fkk")
  | `Thousands => population /. 1000. |> Printf.sprintf("%.1fk")

  | `Hundreds => population |> Printf.sprintf("%.0f")
  };
};

let rec classNames = classes => {
  switch (classes) {
  | [] => ""
  | [x] => x
  | [x, ...rest] => x ++ " " ++ classNames(rest)
  };
};
