.PHONY: all
# all: main proposal
all: main

.PHONY: main
# OUT_DIR is target-specific variable
main: OUT_DIR=./dist/main
# First, we clone directory tree to the output dir so when we use the
# "output-directory" option won't fail (the tex file may write aux files in
# dependecy dir). Finally, remove empty dir (tex file has no dependency)
main:
	find . -type d | xargs -I{} mkdir -p "$(OUT_DIR)/{}"

	pdflatex -output-directory=$(OUT_DIR) main.tex

	cp ./References/references.bib $(OUT_DIR)/References/ && cd $(OUT_DIR) && bibtex ./main

	pdflatex -output-directory=$(OUT_DIR) ./main.tex

	pdflatex -output-directory=$(OUT_DIR) ./main.tex

	find $(OUT_DIR) -type d -empty -delete

# .PHONY: proposal
# proposal: OUT_DIR=./dist/proposal
# proposal:
# 	find . -type d | xargs -I{} mkdir -p "$(OUT_DIR)/{}"

# 	pdflatex -output-directory=$(OUT_DIR) ./proposal.tex

# 	cp ./sample.bib $(OUT_DIR) && cd $(OUT_DIR) && bibtex ./proposal

# 	pdflatex -output-directory=$(OUT_DIR) ./proposal.tex

# 	pdflatex -output-directory=$(OUT_DIR) ./proposal.tex

# 	find $(OUT_DIR) -type d -empty -delete

# Use latexindent to format tex file.
# NOTE: We move the backup files to the backup dir to avoid cluttering the
# source dir.
# NOTE: The settings file is used to configure the latexindent is specified in
# the latexindent.yaml (default) file, you can change the settings file by
# specifying the SETTINGS_FILE variable.
.PHONY: pretty
pretty: BACKUP_DIR=./dist/backup
pretty: SETTINGS_FILE=./latexindent.yaml
pretty:
	mkdir -p $(BACKUP_DIR)

	find . -type f -name "*.tex" | while read texFiles; do \
		latexindent --local=$(SETTINGS_FILE) --cruft=$(BACKUP_DIR) --overwrite --silent --modifylinebreaks $$texFiles; \
	done

.PHONY: clean
clean:
	rm -rf ./dist
