
set(MNTR_COMPILER "/home/jsantos/projects/icarusframework/build/debug/mntr")

function(add_mntr target)
	foreach(file ${ARGN})
		string(REPLACE .cpp.html .cpp ff ${file})
		set(${target}-template-files ${${target}-template-files} ${ff})
		execute_process(
			COMMAND ${MNTR_COMPILER} -i "${file}" -o "${ff}"
		)
		add_custom_command(TARGET ${target} PRE_BUILD
			COMMAND ${MNTR_COMPILER} -i "${file}" -o "${ff}"
		)
	endforeach()
	add_library(${target}-templates
		${${target}-template-files}
	)
	target_link_libraries(${target}
		${target}-templates
	)
endfunction(add_mntr)
