#!/usr/bin/env python3
"""Refresh the README progress badges and status table from build/G8MJ01/report.json.

Run after `python -m ninja` (which writes the report):  python tools/update_readme.py
"""
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPORT = os.path.join(ROOT, "build", "G8MJ01", "report.json")
README = os.path.join(ROOT, "README.md")


def pct(m, key):
    return float(m.get(key, 0) or 0)


def badge(label, value, color="blue"):
    text = f"{value:.2f}%25"
    return f"https://img.shields.io/badge/{label}-{text}-{color}"


def true_c(report):
    """True C figures (L 2026-10-19): the report counts asm stand-ins as matched, so lead with C-only numbers.
    Uses scratch/true_c.py when present; returns {} otherwise."""
    try:
        import importlib.util
        spec = importlib.util.spec_from_file_location("true_c", os.path.join(ROOT, "scratch", "true_c.py"))
        mod = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(mod)
        return mod.stats(report)
    except Exception as e:
        print(f"true C stats unavailable: {e}")
        return {}


def main():
    report = json.load(open(REPORT, encoding="utf-8"))
    total = report["measures"]
    cats = {c["name"]: c["measures"] for c in report.get("categories", [])}
    readme = open(README, encoding="utf-8").read()
    tc = true_c(report)

    # L 2026-10-19: badges lead with true C; the report's figures (asm stand-ins counted as matched) are labelled "Build"
    badges = {
        "Progress": badge("Code", tc["true_code"]) if tc.get("asm_fns", 1) == 0
        else badge("C%20exact", tc["c_exact_code"]) if "c_exact_code" in tc
        else badge("True%20C%20exact", tc["true_code"]) if "true_code" in tc
        else badge("Build", pct(total, "matched_code_percent")),
        "Functions Progress": badge("Functions", 100.0 * tc["true_fns"] / tc["total_fns"]) if tc.get("asm_fns", 1) == 0
        else badge("True%20C%20fuzzy", tc["true_fuzzy"]) if "true_fuzzy" in tc
        else badge("DOL", pct(cats.get("DOL", {}), "matched_code_percent")),
        "Data Progress": badge("Data", tc["true_data"]) if tc.get("asm_fns", 1) == 0 and "true_data" in tc
        else badge("Build%20(with%20asm%20stand--ins)", pct(total, "matched_code_percent"), "lightgrey"),
        "Linked Progress": badge("Linked", pct(total, "complete_code_percent")) if tc.get("asm_fns", 1) == 0
        else badge("Build%20linked", pct(total, "complete_code_percent"), "lightgrey"),
    }
    for name, url in badges.items():
        line = f"[{name}]: {url}"
        if re.search(rf"^\[{re.escape(name)}\]: .*$", readme, re.M):
            readme = re.sub(rf"^\[{re.escape(name)}\]: .*$", line, readme, count=1, flags=re.M)
        else:
            readme = re.sub(r"^(\[Build Status\]: .*)$", lambda m: m.group(1) + "\n" + line, readme, count=1, flags=re.M)
    if "![Linked Progress]" not in readme:
        readme = readme.replace("![Data Progress]", "![Data Progress] ![Linked Progress]", 1)

    rows = [("All", total)] + [(n, m) for n, m in cats.items()]
    table = [
        "| Category | Matched code | Linked code | Linked files | Matched functions |",
        "|---|---|---|---|---|",
    ]
    for name, m in rows:
        units = f"{m.get('complete_units', 0)} / {m.get('total_units', 0)}"
        funcs = f"{m.get('matched_functions', 0)} / {m.get('total_functions', 0)}"
        table.append(
            f"| {name} | {pct(m, 'matched_code_percent'):.2f}% | {pct(m, 'complete_code_percent'):.2f}% "
            f"| {units} | {funcs} |"
        )
    head = ""
    pure_c = tc.get("asm_fns", 1) == 0  # L 2026-10-21: asm stand-ins removed; every figure is C
    if pure_c:
        head = (
            "**Progress** (every unit is built from C and both builds are byte-identical to the original game):\n\n"
            "| Measure | Value |\n|---|---|\n"
            f"| **Code matched by C** (by code size) | **{tc['true_code']:.2f}%** |\n"
            f"| **Functions matched by C** | **{tc['true_fns']} / {tc['total_fns']}** ({100.0 * tc['true_fns'] / tc['total_fns']:.1f}%) |\n"
            f"| Files linked from C | {total.get('complete_units')} / {total.get('total_units')} ({pct(total, 'complete_code_percent'):.2f}% of code) |\n"
            + (f"| Data, exact (bytes + where each relocation points; objdiff's name-paired figure is {tc['objdiff_data']:.2f}%) | {tc['true_data']:.2f}% |\n" if "true_data" in tc else "")
            + "\n"
        )
    elif "true_code" in tc:
        head = (
            "**True C progress** (asm stand-ins are not counted as matched):\n\n"
            "| Measure | Value |\n|---|---|\n"
            + (f"| **C exact** (by code size; includes C that matches but is still built from asm) | **{tc['c_exact_code']:.2f}%** |\n"
               f"| **C exact functions** | **{tc['c_exact_fns']} / {tc['total_fns']}** ({100.0 * tc['c_exact_fns'] / tc['total_fns']:.1f}%) |\n"
               if "c_exact_code" in tc else "")
            + f"| C in the build, exact (by code size) | {tc['true_code']:.2f}% |\n"
            + (f"| True C, fuzzy (asm stand-ins scored by their C) | {tc['true_fuzzy']:.2f}% |\n" if "true_fuzzy" in tc else "")
            + (f"| Data, exact (bytes + where each relocation points; objdiff's name-paired figure is {tc['objdiff_data']:.2f}%) | {tc['true_data']:.2f}% |\n" if "true_data" in tc else "")
            + f"| Functions built from exact C | {tc['c_fns']} / {tc['total_fns']} |\n"
            f"| Functions still asm stand-ins | {tc['asm_fns']}"
            + (f" ({tc['needed_fns']} needed, {tc['paved_fns']} over matching C) |\n\n" if "needed_fns" in tc else " |\n\n")
        )
    us = ""
    us_report = os.path.join(ROOT, "build", "G8ME01", "report.json")
    if os.path.exists(us_report):  # US (G8ME01) figures, once that version has been built
        ur = json.load(open(us_report, encoding="utf-8"))
        um = ur["measures"]
        urows = [("All", um)] + [(c["name"], c["measures"]) for c in ur.get("categories", [])]
        utable = table[:2] + [
            f"| {n} | {pct(m, 'matched_code_percent'):.2f}% | {pct(m, 'complete_code_percent'):.2f}% "
            f"| {m.get('complete_units', 0)} / {m.get('total_units', 0)} "
            f"| {m.get('matched_functions', 0)} / {m.get('total_functions', 0)} |"
            for n, m in urows
        ]
        us = (f"\n\n**US (G8ME01)**, data matched {pct(um, 'matched_data_percent'):.2f}%:\n\n"
              + "\n".join(utable))
    section = (
        "<!-- progress-start -->\n"
        + head
        + ("Per category, JP (G8MJ01) (generated by `tools/update_readme.py`):\n\n" if pure_c else
           "Build report (generated by `tools/update_readme.py`). These columns count asm stand-ins as matched, "
           "so they show that the build is complete and byte-identical, not C progress:\n\n")
        + "\n".join(table)
        + us
        + "\n<!-- progress-end -->"
    )
    if "<!-- progress-start -->" in readme:
        readme = re.sub(r"<!-- progress-start -->.*?<!-- progress-end -->", section, readme, flags=re.S)
    else:
        anchor = "A work-in-progress decompilation of Paper Mario: The Thousand-Year Door.\n"
        readme = readme.replace(anchor, anchor + "\n" + section + "\n", 1)
    open(README, "w", encoding="utf-8", newline="\n").write(readme)
    print(f"README updated: true C {tc.get('true_code', 0):.2f}% exact / {tc.get('true_fuzzy', 0):.2f}% fuzzy, "
          f"build {pct(total, 'matched_code_percent'):.2f}%, "
          f"linked {total.get('complete_units')} / {total.get('total_units')} files")


if __name__ == "__main__":
    sys.exit(main())
