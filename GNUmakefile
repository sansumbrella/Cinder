#!gmake 

.PHONY: debug release debugConfig releaseConfig cleanAll cleanDebug cleanRelease all

BUILD ?= release
CMAKE ?= cmake
OUTPUT_ROOT_DIR ?= CMake
DEBUG_DIR ?= $(OUTPUT_ROOT_DIR)/CinderLinuxD
RELEASE_DIR ?= $(OUTPUT_ROOT_DIR)/CinderLinux
LIBS_DIR ?= $(OUTPUT_ROOT_DIR)/libs

default: $(BUILD)

.DEFAULT:
	@$(MAKE) -C $(BUILD) $(MAKECMDGOALS)

clobber:
	rm -rf $(DEBUG_DIR) $(RELEASE_DIR) $(LIBS_DIR)

all: debugConfig releaseConfig 
	@$(MAKE) -C $(DEBUG_DIR)
	@$(MAKE) -C $(RELEASE_DIR)

clean:
	@$(MAKE) -C $(DEBUG_DIR) clean
	@$(MAKE) -C $(RELEASE_DIR) clean

cleanDebug:
	@$(MAKE) -C $(DEBUG_DIR) clean

cleanRelease:
	@$(MAKE) -C $(RELEASE_DIR) clean

#debugSyncData:
	#@$(CMAKE) -E copy_directory data/ debug/bin/data

#releaseSyncData:
	#@$(CMAKE) -E copy_directory data/ release/bin/data

debugConfig: 
	@mkdir -p $(DEBUG_DIR)
	@cd $(DEBUG_DIR); $(CMAKE) ../..  -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles"

debug: debug/Makefile
	@$(MAKE) -C $(DEBUG_DIR) 

debug/Makefile:
	@mkdir -p $(DEBUG_DIR)
	@cd $(DEBUG_DIR); $(CMAKE) ../..  -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles"

releaseConfig:
	@mkdir -p $(RELEASE_DIR)
	@cd $(RELEASE_DIR); $(CMAKE) ../.. -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles"

release: release/Makefile 
	@$(MAKE) -C $(RELEASE_DIR) 

release/Makefile:
	@mkdir -p $(RELEASE_DIR)
	@cd $(RELEASE_DIR); $(CMAKE) ../..  -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles"
	
