"""Check one implementation per migrated file and byte-preserving forwarders."""
import hashlib
import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

root = Path(__file__).resolve().parents[1]
manifest = json.loads((root / "cmake/source-layout.json").read_text())
assert manifest["schema_version"] == 1
seen = set()
for entry in manifest["files"]:
    legacy, canonical = root / entry["legacy"], root / entry["canonical"]
    assert entry["canonical"] not in seen, entry
    seen.add(entry["canonical"])
    relative = os.path.relpath(canonical, legacy.parent).replace(os.sep, "/")
    expected = (f'/* Compatibility forwarding file. Canonical implementation: {entry["canonical"]}.\n'
                ' * Select this source OR its canonical source, never both. See README.md. */\n'
                f'#include "{relative}"\n')
    assert legacy.read_text() == expected, f"implementation drift in {legacy}"
    assert hashlib.sha256(canonical.read_bytes()).hexdigest() == entry["sha256"], canonical
assert len(seen) == 98
print(f"{len(seen)} canonical files and legacy forwarders verified")

# OV2640 historically expects a consumer-supplied Common/camera.h. Relocation
# must keep that include-root contract, without inventing a board camera port.
with tempfile.TemporaryDirectory() as directory:
    fixture = Path(directory)
    (fixture / "Common").mkdir()
    (fixture / "Common/camera.h").write_text("#include <stdint.h>\n#define CAMERA_PORT_TEST 123\n")
    source = fixture / "check.c"
    source.write_text('#include <Camera/ov2640.h>\n#if CAMERA_PORT_TEST != 123\n#error missing consumer camera port\n#endif\n')
    for include in (root / "Inc", root / "Drivers/Camera/Inc"):
        subprocess.run([sys.argv[1], "-E", "-P", "-I", str(include), "-I", str(fixture),
                        str(source), "-o", os.devnull], check=True)
