open Jest;

Expect.(
  describe("getUnit", () => {
    test("must give `Millions unit", () => {
      let unit = Shared_js_demo.Utils.getUnit(1000000.);

      expect(unit) |> toBe(`Millions);
    });

    test("must give `Thousands unit", () => {
      let unit = Shared_js_demo.Utils.getUnit(1000.);

      expect(unit) |> toBe(`Thousands);
    });

    test("must give `Hundreds unit", () => {
      let unit = Shared_js_demo.Utils.getUnit(1.);

      expect(unit) |> toBe(`Hundreds);
    });
  })
);

Expect.(
  describe("formatPopulation", () => {
    test("must format millions", () => {
      let str = Shared_js_demo.Utils.formatPopulation(1000000.);

      expect(str) |> toBe("1.0kk");
    });

    test("must format thousands", () => {
      let str = Shared_js_demo.Utils.formatPopulation(1000.);

      expect(str) |> toBe("1.0k");
    });

    test("must format hundreds", () => {
      let str = Shared_js_demo.Utils.formatPopulation(100.);

      expect(str) |> toBe("100");
    });
  })
);

Expect.(
  describe("classNames", () => {
    test("must create classNames", () => {
      let str = Shared_js_demo.Utils.classNames(["a", "b", "c"]);

      expect(str) |> toBe("a b c");
    })
  })
);
