if (CONFIG_BUGIS_DRIVER)    
    target_include_directories(app PRIVATE ../../include)
    target_sources(app PRIVATE bugis_driver.c)
endif()