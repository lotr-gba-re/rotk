# carved-local/

Art assets carved from the ROM into editable formats (graphics, sound, animations, ...) that stay out of the committed repository.
You must carve the contents from an original ROM using `make carve`.
Enemy animation `metadata.json` files and frame PNGs generate their C sources under `build/generated/` without reading the ROM.
`make clean` preserves these inputs; `make clean-local` deletes them.
