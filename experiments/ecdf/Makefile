all: default-parameters run doc

default-parameters:
	hnco --print-default-parameters > log.default

run:
	echo `date` >> log.err
	time -o log.err -a hnco-ecdf-run.py

doc:
	sed '/^$$/d' plan.json > plan-compact.json
#	make -C report

clean:
	rm -f plan-compact.json log.default log.err
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run doc clean
