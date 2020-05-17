all: default-parameters run report

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-dynamics-skeleton.pl
	time -o log.err -a hnco-dynamics-run.pl

report:
	hnco-dynamics-stat.pl
	./graphics.gp
	sed '/^$$/d' plan.json > pretty.json
	make -C report

clean:
	rm -f graphics.gp pretty.json log.defaults log.err
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run doc clean
