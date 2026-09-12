"""PyGhidra JVM environment for the batch scripts: the GHIDRA_INSTALL_DIR default and
headless AWT via _JAVA_OPTIONS. Import for the side effects before the JVM starts.
"""

import os

os.environ.setdefault("GHIDRA_INSTALL_DIR", "/opt/ghidra")

# Append so an existing _JAVA_OPTIONS is preserved; headless is the last token, so it wins.
os.environ["_JAVA_OPTIONS"] = (
    os.environ.get("_JAVA_OPTIONS", "") + " -Djava.awt.headless=true"
).strip()
