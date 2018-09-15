all: default-parameters run doc

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-benchmark-skeleton.pl
	time -o log.err -a hnco-benchmark-run.pl

doc:
	hnco-ecdf-stat.pl
	./graphics.gp
	./graphics-all.gp
	sed '/^$$/d' plan.json > plan-compact.json
	make -C report

clean:
	rm -f graphics.gp graphics-all.gp plan-compact.json log.defaults log.err
	rm -rf results
	rm -rf graphics
	rm -rf commands.txt

.PHONY: all default-parameters run doc clean
