# eyes

Files in this section: `README.md`, `eyes.h`, `eyes.c`, `eyes_map.c`,
`page_picture.txt`, `output/`

## Purpose (do not rewrite this plan)

Two sides of the same sight:

1. **Model / rebuild** — deposit pixels as binary marks, rebuild, measure
   pixel convergence. Error = any pixel that cannot rebuild.
2. **She / picture** — while the page shows, she sees the picture and
   learns color subconsciously. Color is not a separate drill here.

A pixel is a pixel. One honest collect → rebuild → diff. No 20× forever
loop. Do not swap this plan for harness theater or new theories.

## What works now

- Load a real palette picture file (`page_picture.txt`).
- Mono pull (ink bit from luma).
- Deposit five marks per pixel: `R G B A BIT`.
- Read the deposit back from disk and rebuild.
- Print true `CANNOT REBUILD FROM DEPOSIT` and `BIT MARK ALONE LOSES`.
- Print `hello` when full deposit rebuild loses zero pixels.

## NOT DONE

- Host camera / live world pixels (Swift host later through MCP).
- Text page print path (font stamp) — not in this step.
- Screen surface, hearing, sense, MCP — later move-order steps.
- Do not treat mono BIT alone as full sight; color/gray need the channel marks.

## Build

From repository root:

```
make eyes
```

Builds `eyes/eyes_map` and runs one collect → rebuild → diff on
`eyes/page_picture.txt`. Writes `eyes/output/page1_deposit.txt`.
