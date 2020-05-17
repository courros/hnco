all: default-parameters run report

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-benchmark-skeleton.pl
	time -o log.err -a hnco-benchmark-run.pl

report:
	hnco-ecdf-stat.pl
	./graphics.gp
	./graphics-all.gp
	sed '/^$$/d' plan.json > pretty.json
	make -C report

clean:
	rm -f pretty.json
	rm -f log.defaults log.err log.parallel
	rm -f commands.txt
	rm -f graphics.gp graphics-all.gp
	rm -rf graphics
	rm -rf results

.PHONY: all default-parameters run doc clean
