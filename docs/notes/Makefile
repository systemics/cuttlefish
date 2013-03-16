.SUFFIXES:
.PHONY: default
default: markdown

markdown: TODO.html NOTES.html

clean:
	rm -rf *.html

# rule for making .html from markdown
#
%.html :: %.md
	markdown $^ > $@

