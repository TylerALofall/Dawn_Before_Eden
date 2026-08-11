# eyes

Files in this section: `README.md`, `eyes.h`, `eyes.c`, `eyes_map.c`,
`page_picture.txt`, `output/`

## Rule (truce)

A pixel is a pixel. Copy pixels off the document as binary marks. Rebuild
the page from those marks. Error = any pixel that cannot rebuild.

One honest pass is enough. No 20× forever loop.

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
