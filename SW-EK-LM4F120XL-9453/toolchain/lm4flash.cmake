set(BINARY ${PROJECT_NAME}.bin)

add_custom_target(flash 
	COMMAND lm4flash ${PROJECT_NAME}.bin
        DEPENDS ${PROJECT_NAME}
	)