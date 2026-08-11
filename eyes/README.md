# eyes

Files in this section: `README.md`, `eyes.h`, `eyes.c`, `eyes_map.c`,
`page_picture.txt`, `output/`

## Mechanical rule (end of story)

No subprocessor may touch the pixels. One core. One pass.
Zero black box: every mark and rule is open — no hidden math.

A pixel is a pixel. Deposit the marks. Rebuild from the marks.
Error = any pixel that cannot rebuild.

Binary color deposit is this path's body (pretraining ground).
Later simultaneous sight + wake is a later step — not a second core here.
20× exists only to find mistakes later — not a forever-proof hunt.

## What works now

- Load a real palette picture file (`page_picture.txt`).
- One mechanical deposit walk: five marks per pixel `R G B A BIT`
  (BIT taken from that same pixel in the same walk).
- Read the deposit back from disk and rebuild RGBA from the marks.
- Print true `CANNOT REBUILD FROM DEPOSIT`.
- Print `hello` when full deposit rebuild loses zero pixels.

## NOT DONE

- Host camera / live world pixels (Swift host later through MCP).
- Simultaneous sight + wake (later step).
- 20× mistake finder (later; not forever theater).
- Text page print path (font stamp) — not in this step.
- Screen surface, hearing, sense, MCP — later move-order steps.

## Build

From repository root:

```
make eyes
```

Builds `eyes/eyes_map` and runs one deposit → rebuild → diff on
`eyes/page_picture.txt`. Writes `eyes/output/page1_deposit.txt`.
