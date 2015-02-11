FUNCTION(RMSC_COMMAND name header)
	# get prefix from input filename
	GET_FILENAME_COMPONENT(prefix ${header} NAME_WE)

	# get output filename and set global output name
	SET(outfile "${CMAKE_BINARY_DIR}/${prefix}_json.c")
	SET(RMSC_${name}_OUTPUTS "${outfile}" PARENT_SCOPE)

	# trigger restful meta struct compiler
	ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
		COMMAND rmsc -i ${header} -o ${outfile}
	)
ENDFUNCTION()
