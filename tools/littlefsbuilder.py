Import("env")
env.Replace( MKSPIFFTOOL=env.get("PROJECT_DIR") + '/tools/mklittlefs' )  # PlatformIO now believes it has actually created a SPIFFS
