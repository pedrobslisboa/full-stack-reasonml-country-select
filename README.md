# Fullstack Reasonml Country Select

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Improvements](#improvements)

## Introduction

This is a simple country select component built with Reasonml and React. It uses the [gist API](https://gist.githubusercontent.com/pedrobslisboa/1a6af944bc5f70067018df6f0ab30263/raw/f987794a5be8bb40bb92595f615024a59123a6dc/country-list-population.json) to fetch the list of countries.

It work as a sample project to show how to build a simple component with Reasonml and React, with a native Reasonml server.

This project uses the following main libraries:

- reasnml
- reason-react
- server-reason-react
- melange

## Installation

```sh
    Make install
```

With docker:

```sh
    Make docker-build
```

## Usage

```sh
    Make run
```

With docker:

```sh
    Make docker-run
```

## License

MIT

## Improvements

- Increase test coverage.
- Increase custom Bindings.
- Usage of UniversalPortal for the Select dropdown, avoiding problems with z-index and overflow.
- CSS modules or Styled components for better styling. (Maybe styled-ppx)
- Better error handling on server side.
- Create useCombobox hook to handle the select logic, the Select component has too much logic.

Something like:

```
type useCombobox: (~options: list('a), onActiveIndex: int => unit, ~onSelect: 'a => unit, ~onOpen: unit => unit, ~onClose: unit=> unit) => {
    activeIndex: int,
    isOpen: bool,
    open: unit => unit,
    close: unit => unit,
    inputProps: {|..|},
    listboxProps: {|..|},
    optionProps: int => {|..|},
}
```

