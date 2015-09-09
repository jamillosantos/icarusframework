

function(do_stuff arg1 arg2)
	foreach(f ${ARGN})
		# do something interesting with file 'f'
		message("f='${f}' arg1='${arg1}' arg2='${arg2}'")
	endforeach()
	message("f='${f}' arg1='${arg1}' arg2='${arg2}' ARGV2='${ARGV2}'")
endfunction()

set(MNTR_COMPILER echo)

function(add_mntr target input_directory output_directory)
	get_filename_component(id_abs ${input_directory} ABSOLUTE)
	get_filename_component(od_abs ${output_directory} ABSOLUTE)
	execute_process(
		COMMAND ${MNTR_COMPILER} --input-dir "${id_abs}" --output-dir ${od_abs} ${file_abs}
		RESULT_VARIABLE MNTR_EXIT_STATUS
	)
#	foreach(file ${ARGN})
#		get_filename_component(file_abs ${file} ABSOLUTE)
#		message(
#			COMMAND ${MNTR_COMPILER} --input-dir "${id_abs}" --output-dir ${od_abs} ${file_abs}
#			RESULT_VARIABLE MNTR_EXIT_STATUS
#		)
#		execute_process(
#			COMMAND ${MNTR_COMPILER} --input-dir "${id_abs}" --output-dir ${od_abs} ${file_abs}
#			RESULT_VARIABLE MNTR_EXIT_STATUS
#		)
#	endforeach()
endfunction(add_mntr)
