# 3D Renderer

Este é um projeto educacional de renderização 3D utilizando **SDL3** como base para visualização gráfica. O projeto está sendo desenvolvido com base na disciplina de **Computação Gráfica** e no livro:

**Computação Gráfica Vol. 2 – Teoria e Prática: Geração de Imagens**  
**Autores:** Eduardo Azevedo, Aura Conci e Fabiana Leta

---

## Sobre o Projeto

O objetivo deste projeto é implementar, a partir do zero, um pipeline gráfico 3D básico, abordando:

-   Representação de objetos 3D por vértices e arestas;
-   Transformações geométricas (translação, rotação, escala);
-   Projeção em perspectiva;
-   Rasterização simples;
-   Visualização interativa.

A biblioteca **SDL3** (Simple DirectMedia Layer) é usada para criação de janelas e gerenciamento de gráficos e eventos.

---

## Pré-requisitos

Antes de compilar este projeto, certifique-se de ter os seguintes itens instalados:

-   [CMake >= 3.16](https://cmake.org/download/)
-   Um compilador C/C++ compatível (GCC, Clang, MSVC etc.)

---

## Instruções de Uso

### 1. Clone este repositório

```bash
git clone https://github.com/gabrielbrev/3D-Renderer.git
cd 3D-Renderer
```

### 2. Clone o SDL3 como subdiretório

Este projeto não inclui o SDL3 como submódulo. Você precisa cloná-lo manualmente dentro do projeto:

```bash
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
```

> O diretório `vendored/SDL` será usado pelo CMake para compilar o SDL junto com o projeto.

### 3. Compile o projeto

```bash
cmake -S . -B build
cmake --build build
```

### 4. Execute o programa

O executável deve estar no diretório `build`:

```bash
cd build
./main
```

Se não houver um executável ali, mesmo após a etapa de **Build** ter sido concluída com sucesso, ele deve estar em `build/Debug`:

```bash
cd build/Debug
./main
```
