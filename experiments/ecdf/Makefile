all: default-parameters run doc

default-parameters:
	hnco --print-default-parameters > log.default

run:
	echo `date` >> log.err
	time -o log.err -a hnco-benchmark-run.pl

doc:
	hnco-ecdf-stat.pl
	./graphics.gp
	./graphics-all.gp
	sed '/^$$/d' plan.json > plan-compact.json
	make -C report

clean:
	rm -f graphics.gp plan-compact.json log.default log.err
	rm -rf results
	rm -rf graphics
	rm -rf commands.txt

.PHONY: all default-parameters run doc clean
