#!/usr/bin/env python3
"""GUI for browsing app categories and editing per-app category assignments.

Scans every immediate subdirectory of the repository root for a
metadata.json file, lets you see which categories exist and how many
apps use each one, and lets you check/uncheck categories for an
individual app and save the result back to its metadata.json.

Run from anywhere:
    python3 .tools/category_manager.py [repo_root]

If repo_root is omitted it defaults to the parent directory of this
script (i.e. the repository root, assuming this file stays in .tools/).
"""

import json
import re
import sys
import tkinter as tk
from pathlib import Path
from tkinter import messagebox, ttk

IGNORED_DIR_PREFIXES = (".",)
DEFAULT_INDENT = 4
UNCATEGORIZED_IID = "\x00uncategorized\x00"
UNCATEGORIZED_LABEL = "(no category)"


def detect_indent(text: str) -> int:
    """Guess the indent width (in spaces) used by an existing JSON file."""
    match = re.search(r"\n( +)\S", text)
    return len(match.group(1)) if match else DEFAULT_INDENT


class App:
    def __init__(self, slug, path, data, indent=DEFAULT_INDENT, trailing_newline=True):
        self.slug = slug
        self.path = path
        self.data = data
        self.indent = indent
        self.trailing_newline = trailing_newline

    @property
    def categories(self):
        return self.data.get("categories", [])

    @categories.setter
    def categories(self, value):
        self.data["categories"] = value

    @property
    def name(self):
        return self.data.get("name", self.slug)


def scan_apps(root: Path):
    """Return (apps, errors). apps: slug -> App. errors: slug -> message."""
    apps = {}
    errors = {}
    for entry in sorted(root.iterdir()):
        if not entry.is_dir():
            continue
        if entry.name.startswith(IGNORED_DIR_PREFIXES):
            continue
        metadata_path = entry / "metadata.json"
        if not metadata_path.is_file():
            continue
        try:
            text = metadata_path.read_text(encoding="utf-8")
            data = json.loads(text)
        except (OSError, json.JSONDecodeError) as exc:
            errors[entry.name] = str(exc)
            continue
        apps[entry.name] = App(
            entry.name,
            metadata_path,
            data,
            indent=detect_indent(text),
            trailing_newline=text.endswith("\n"),
        )
    return apps, errors


def save_app(app: App):
    with open(app.path, "w", encoding="utf-8") as f:
        json.dump(app.data, f, indent=app.indent)
        if app.trailing_newline:
            f.write("\n")


class CategoryManagerGUI:
    def __init__(self, master, root_dir: Path):
        self.master = master
        self.root_dir = root_dir
        self.apps = {}
        self.errors = {}
        self.all_categories = set()
        self.category_filter = None
        self.current_slug = None
        self.check_vars = {}

        master.title(f"App category manager - {root_dir}")
        master.geometry("1150x650")

        self._build_widgets()
        self.reload()

    # ----- widget construction -------------------------------------------------
    def _build_widgets(self):
        toolbar = ttk.Frame(self.master)
        toolbar.pack(side=tk.TOP, fill=tk.X, padx=6, pady=4)
        ttk.Button(toolbar, text="Reload", command=self.reload).pack(side=tk.LEFT)
        ttk.Button(toolbar, text="Clear category filter", command=self.clear_filter).pack(
            side=tk.LEFT, padx=(6, 0)
        )
        self.status_var = tk.StringVar()
        ttk.Label(toolbar, textvariable=self.status_var).pack(side=tk.LEFT, padx=12)

        paned = ttk.PanedWindow(self.master, orient=tk.HORIZONTAL)
        paned.pack(side=tk.TOP, fill=tk.BOTH, expand=True, padx=6, pady=(0, 6))

        # --- Categories pane ---
        cat_frame = ttk.Frame(paned)
        ttk.Label(cat_frame, text="Categories", font=("", 10, "bold")).pack(
            anchor=tk.W, padx=4, pady=(4, 0)
        )
        self.cat_tree = ttk.Treeview(
            cat_frame, columns=("category", "count"), show="headings", selectmode="browse"
        )
        self.cat_tree.heading("category", text="Category")
        self.cat_tree.heading("count", text="Apps")
        self.cat_tree.column("category", width=140, anchor=tk.W)
        self.cat_tree.column("count", width=50, anchor=tk.CENTER)
        self.cat_tree.pack(side=tk.TOP, fill=tk.BOTH, expand=True, padx=4, pady=4)
        self.cat_tree.bind("<<TreeviewSelect>>", self.on_category_select)
        paned.add(cat_frame, weight=1)

        # --- Apps pane ---
        app_frame = ttk.Frame(paned)
        self.app_list_label = ttk.Label(app_frame, text="Apps", font=("", 10, "bold"))
        self.app_list_label.pack(anchor=tk.W, padx=4, pady=(4, 0))
        self.app_tree = ttk.Treeview(
            app_frame, columns=("slug", "name", "categories"), show="headings", selectmode="browse"
        )
        self.app_tree.heading("slug", text="Slug")
        self.app_tree.heading("name", text="Name")
        self.app_tree.heading("categories", text="Categories")
        self.app_tree.column("slug", width=220, anchor=tk.W)
        self.app_tree.column("name", width=180, anchor=tk.W)
        self.app_tree.column("categories", width=220, anchor=tk.W)
        self.app_tree.pack(side=tk.TOP, fill=tk.BOTH, expand=True, padx=4, pady=4)
        self.app_tree.bind("<<TreeviewSelect>>", self.on_app_select)
        paned.add(app_frame, weight=2)

        # --- Editor pane ---
        edit_frame = ttk.Frame(paned)
        self.edit_title = ttk.Label(edit_frame, text="Select an app", font=("", 10, "bold"))
        self.edit_title.pack(anchor=tk.W, padx=4, pady=(4, 0))

        canvas_container = ttk.Frame(edit_frame)
        canvas_container.pack(side=tk.TOP, fill=tk.BOTH, expand=True, padx=4, pady=4)
        self.checks_canvas = tk.Canvas(canvas_container, highlightthickness=0)
        scrollbar = ttk.Scrollbar(
            canvas_container, orient=tk.VERTICAL, command=self.checks_canvas.yview
        )
        self.checks_canvas.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.checks_canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.checks_inner = ttk.Frame(self.checks_canvas)
        self._checks_inner_id = self.checks_canvas.create_window(
            (0, 0), window=self.checks_inner, anchor="nw"
        )
        self.checks_inner.bind(
            "<Configure>",
            lambda e: self.checks_canvas.configure(scrollregion=self.checks_canvas.bbox("all")),
        )
        self.checks_canvas.bind(
            "<Configure>",
            lambda e: self.checks_canvas.itemconfig(self._checks_inner_id, width=e.width),
        )

        add_frame = ttk.Frame(edit_frame)
        add_frame.pack(side=tk.TOP, fill=tk.X, padx=4, pady=(0, 4))
        ttk.Label(add_frame, text="New category:").pack(side=tk.LEFT)
        self.new_cat_var = tk.StringVar()
        new_cat_entry = ttk.Entry(add_frame, textvariable=self.new_cat_var)
        new_cat_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=4)
        new_cat_entry.bind("<Return>", lambda e: self.add_new_category())
        ttk.Button(add_frame, text="Add", command=self.add_new_category).pack(side=tk.LEFT)

        save_frame = ttk.Frame(edit_frame)
        save_frame.pack(side=tk.TOP, fill=tk.X, padx=4, pady=(0, 4))
        self.save_button = ttk.Button(
            save_frame, text="Save changes", command=self.save_current, state=tk.DISABLED
        )
        self.save_button.pack(side=tk.LEFT)
        self.edit_status_var = tk.StringVar()
        ttk.Label(save_frame, textvariable=self.edit_status_var).pack(side=tk.LEFT, padx=8)

        paned.add(edit_frame, weight=1)

    # ----- data loading -------------------------------------------------
    def reload(self):
        self.apps, self.errors = scan_apps(self.root_dir)
        self.all_categories = set()
        for app in self.apps.values():
            self.all_categories.update(app.categories)
        self.category_filter = None
        self.current_slug = None
        self._refresh_category_tree()
        self._refresh_app_tree()
        self._clear_editor()
        msg = f"{len(self.apps)} apps loaded"
        if self.errors:
            msg += f", {len(self.errors)} could not be parsed (see console)"
            for slug, err in self.errors.items():
                print(f"Failed to parse {slug}/metadata.json: {err}", file=sys.stderr)
        self.status_var.set(msg)

    def _refresh_category_tree(self):
        self.cat_tree.delete(*self.cat_tree.get_children())
        counts = {}
        uncategorized_count = 0
        for app in self.apps.values():
            if app.categories:
                for c in app.categories:
                    counts[c] = counts.get(c, 0) + 1
            else:
                uncategorized_count += 1
        self.cat_tree.insert(
            "",
            tk.END,
            iid=UNCATEGORIZED_IID,
            values=(UNCATEGORIZED_LABEL, uncategorized_count),
            tags=("uncategorized",),
        )
        self.cat_tree.tag_configure("uncategorized", foreground="#555555")
        for cat in sorted(self.all_categories, key=str.lower):
            self.cat_tree.insert("", tk.END, iid=cat, values=(cat, counts.get(cat, 0)))

    def _refresh_app_tree(self):
        self.app_tree.delete(*self.app_tree.get_children())
        for slug, app in sorted(self.apps.items(), key=lambda kv: kv[0].lower()):
            if self.category_filter == UNCATEGORIZED_IID:
                if app.categories:
                    continue
            elif self.category_filter and self.category_filter not in app.categories:
                continue
            self.app_tree.insert(
                "",
                tk.END,
                iid=slug,
                values=(slug, app.name, ", ".join(app.categories)),
            )
        if self.category_filter == UNCATEGORIZED_IID:
            self.app_list_label.config(text=f"Apps (filtered: {UNCATEGORIZED_LABEL})")
        elif self.category_filter:
            self.app_list_label.config(text=f"Apps (filtered: {self.category_filter})")
        else:
            self.app_list_label.config(text="Apps")

    # ----- category filtering -------------------------------------------------
    def on_category_select(self, _event):
        sel = self.cat_tree.selection()
        self.category_filter = sel[0] if sel else None
        self._refresh_app_tree()

    def clear_filter(self):
        self.cat_tree.selection_remove(self.cat_tree.selection())
        self.category_filter = None
        self._refresh_app_tree()

    # ----- app selection / editing -------------------------------------------------
    def on_app_select(self, _event):
        sel = self.app_tree.selection()
        if not sel:
            self._clear_editor()
            return
        self._load_editor(sel[0])

    def _clear_editor(self):
        self.current_slug = None
        self.edit_title.config(text="Select an app")
        for child in self.checks_inner.winfo_children():
            child.destroy()
        self.check_vars = {}
        self.save_button.config(state=tk.DISABLED)
        self.edit_status_var.set("")

    def _load_editor(self, slug):
        self.current_slug = slug
        app = self.apps[slug]
        self.edit_title.config(text=f"{app.name}  ({slug})")
        self.edit_status_var.set("")
        for child in self.checks_inner.winfo_children():
            child.destroy()
        self.check_vars = {}
        selected = set(app.categories)
        for cat in sorted(self.all_categories, key=str.lower):
            var = tk.BooleanVar(value=cat in selected)
            cb = ttk.Checkbutton(self.checks_inner, text=cat, variable=var)
            cb.pack(anchor=tk.W, padx=4, pady=1)
            self.check_vars[cat] = var
        self.save_button.config(state=tk.NORMAL)

    def add_new_category(self):
        new_cat = self.new_cat_var.get().strip()
        if not new_cat:
            return
        self.new_cat_var.set("")
        is_new = new_cat not in self.all_categories
        self.all_categories.add(new_cat)
        if is_new:
            self._refresh_category_tree()
        if self.current_slug is not None:
            if new_cat not in self.check_vars:
                var = tk.BooleanVar(value=True)
                cb = ttk.Checkbutton(self.checks_inner, text=new_cat, variable=var)
                cb.pack(anchor=tk.W, padx=4, pady=1)
                self.check_vars[new_cat] = var
            else:
                self.check_vars[new_cat].set(True)

    def save_current(self):
        if self.current_slug is None:
            return
        slug = self.current_slug
        app = self.apps[slug]
        new_categories = sorted(
            (cat for cat, var in self.check_vars.items() if var.get()), key=str.lower
        )
        if not new_categories:
            if not messagebox.askyesno(
                "No categories selected",
                f"'{app.name}' will be saved with an empty categories list. Continue?",
            ):
                return
        app.categories = new_categories
        try:
            save_app(app)
        except OSError as exc:
            messagebox.showerror("Save failed", f"Could not write {app.path}:\n{exc}")
            return
        self.edit_status_var.set("Saved.")
        self._refresh_category_tree()
        self._refresh_app_tree()
        if self.app_tree.exists(slug):
            self.app_tree.selection_set(slug)


def main():
    if len(sys.argv) > 1:
        root_dir = Path(sys.argv[1]).resolve()
    else:
        root_dir = Path(__file__).resolve().parent.parent
    if not root_dir.is_dir():
        print(f"Not a directory: {root_dir}", file=sys.stderr)
        sys.exit(1)

    root = tk.Tk()
    try:
        style = ttk.Style(root)
        if "clam" in style.theme_names():
            style.theme_use("clam")
    except tk.TclError:
        pass
    CategoryManagerGUI(root, root_dir)
    root.mainloop()


if __name__ == "__main__":
    main()
