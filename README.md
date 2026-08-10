# Dawn_Before_Eden

A Final Understanding, Shakti on deck.

Clean cutover home for Shakti. Modules are moved from `shakti_eden-` one controlled set at a time. Authority: Tyler's latest direct instruction, then `SHAKTI_LOCK_V1_1.md`.

## Moved in this cutover

- C99 shell: `include/`, `src/`, `Makefile`, `tools/`, `tests/`
- Eyes module: `eyes/`
- Binary deposit module: `binary/` (source + fixtures)
- House data: `data/`, token media `eden_out/`
- Human docs: `docs/`, `SHAKTI_LOCK_V1_1.md`
- Training order / control: `Ordered_Human_Learning_Map 2.xlsx`, `shakti_control 2.txt`
- Repo guidance: `.github/`

Not moved (left behind on purpose): `old/`, archives/zips, generated run reports, binary deposit run outputs, duplicate constitution dumps, HTML/JS host mock, and other noise.

## Build

```sh
make clean
make
```

## Verify

```sh
make test
./shakti --check
./shakti --demo
```

## Run

```sh
./shakti
```

C99 only inside Shakti's shell. No Python. No JavaScript in the final product. MCP is the gated route out.
