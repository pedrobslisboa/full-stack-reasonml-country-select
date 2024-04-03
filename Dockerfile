# Use uma imagem base do Ubuntu
FROM ubuntu:20.04

# Evitar perguntas da interface de usuário ao instalar pacotes
ARG DEBIAN_FRONTEND=noninteractive

# RUN curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
# RUN echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list

RUN apt-get update && apt-get install -y curl 

RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash -

# Atualizar a lista de pacotes e instalar dependências necessárias
RUN apt-get update && apt-get install -y \
    git \
    m4 \
    make \
    opam \
    nodejs \
    pkg-config \
    libssl-dev \
    libev-dev \
    libpcre3-dev \
    libgmp-dev \
    && rm -rf /var/lib/apt/lists/*

RUN npm install -g yarn

# Adicionar os arquivos do projeto ao container
COPY . .

# Instalar as dependências do projeto
RUN yarn install

# Inicializar o ambiente opam
RUN opam init -y --disable-sandboxing

# Criar um switch opam usando a versão específica do OCaml
RUN opam switch create 5.1.0 && \
    eval $(opam env)

RUN opam install reason.3.10.0 --yes \
    && opam install melange.2.0.0 --yes \
    && opam install melange-fetch --yes \
    && opam install dream --yes \
    && opam install reason-react-ppx --yes \
    && opam install yojson --yes \
    && opam install reason-react.0.13.0 --yes \
    && opam install lwt.5.7.0 --yes \
    && opam install cohttp --yes \
    && opam install cohttp-lwt-unix --yes \
    && opam install melange-testing-library --yes \
    && opam install melange-jest --yes \
    && opam pin add server-reason-react "https://github.com/ml-in-barcelona/server-reason-react.git#d29790ffac08e2f4f3f8d65374ca76ebde334ab3" --yes \
    && opam pin add universal-portal "https://github.com/pedrobslisboa/universal-portal.git#main" --yes

RUN yarn webpack --env production

# Construir o projeto
# Substitua o comando abaixo pelo necessário para construir seu projeto, se diferente
RUN eval $(opam env) && \
    opam exec -- dune build --profile=prod @all

# Expor a porta do servidor
EXPOSE 8000

# CMD ["opam" exec -- dune exec src/server/server.exe"]
CMD ["opam", "exec", "--", "dune", "exec", "src/server/server.exe"]
