NAME		:= tetris
SRC_DIR		:= .
GAME_DIR	:= $(SRC_DIR)/brick_game/tetris
GUI_DIR		:= $(SRC_DIR)/gui/cli
BUILD_DIR	:= build
TEST_DIR	:= tests
OBJ_DIR		:= $(BUILD_DIR)/obj
BIN_DIR		:= $(BUILD_DIR)/bin
TEXDOC 		:= doc/$(NAME).tex
DVIFILE 	:= doc/$(NAME).dvi

CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -std=c11 -pedantic \
          -I$(GUI_DIR) \
          -I$(GAME_DIR) \
          -I$(TEST_DIR)

LDFLAGS		:= -lncursesw -lsqlite3
TEST_LDFLAGS	:= $(LDFLAGS) -lcheck -lpthread -lm -lrt -lsubunit
GCOV_FLAGS	:= -fprofile-arcs -ftest-coverage
GCOV_LDFLAGS := -lgcov

GAME_SRCS	:= $(wildcard $(GAME_DIR)/*.c)
GUI_SRCS	:= $(wildcard $(GUI_DIR)/*.c)
TEST_SRCS	:= $(filter-out $(TEST_DIR)/_test_runner.c, $(wildcard $(TEST_DIR)/*.c))
SRCS		:= $(GAME_SRCS) $(GUI_SRCS)

GAME_OBJS	:= $(patsubst $(GAME_DIR)/%.c,$(OBJ_DIR)/brick_game/tetris/%.o,$(GAME_SRCS))
GUI_OBJS	:= $(patsubst $(GUI_DIR)/%.c,$(OBJ_DIR)/gui/cli/%.o,$(GUI_SRCS))
TEST_OBJS	:= $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/tests/%.o,$(TEST_SRCS))

GUI_SRCS_WITHOUT_MAIN := $(filter-out $(GUI_DIR)/main.c, $(GUI_SRCS))
GUI_OBJS_WITHOUT_MAIN := $(patsubst $(GUI_DIR)/%.c,$(OBJ_DIR)/gui/cli/%.o,$(GUI_SRCS_WITHOUT_MAIN))

OBJS		:= $(GAME_OBJS) $(GUI_OBJS)

TEST_EXEC	:= $(BIN_DIR)/run_tests
TEST_RUNNER	:= $(TEST_DIR)/_test_runner.c

PREFIX		:= .
BINDIR		:= $(PREFIX)/games/$(NAME)

all: $(BIN_DIR)/$(NAME)

$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@
	@mkdir -p $(OBJ_DIR)/brick_game/tetris
	@mkdir -p $(OBJ_DIR)/gui/cli
	@mkdir -p $(OBJ_DIR)/tests

$(BIN_DIR)/$(NAME): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/brick_game/tetris/%.o: $(GAME_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/gui/cli/%.o: $(GUI_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	@mkdir -p $(BINDIR)
	cp $(BIN_DIR)/$(NAME) $(BINDIR)/
	chmod 755 $(BINDIR)/$(NAME)
	@echo "Installed to $(BINDIR)"

uninstall:
	rm -rf $(BINDIR)
	@echo "Uninstalled from $(BINDIR)"

clean:
	rm -rf $(BUILD_DIR) ../*.gcno ../*.gcda ../*.gcov ../gcov_report

clean-doc:
	rm -f doc/*.aux doc/*.log doc/*.toc doc/*.out doc/*.dvi doc/*.ps doc/*.pdf

dvi: $(DVIFILE)
	xdvi $(DVIFILE)

$(DVIFILE): $(TEXDOC)
	cd doc && latex $(NAME).tex
	cd doc && latex $(NAME).tex

dist: clean
	mkdir -p ../$(NAME)-src
	cp -r ../src/brick_game ../src/gui Makefile ../$(NAME)-src/
	tar -czf ../$(NAME).tar.gz ../$(NAME)-src/
	rm -rf ../$(NAME)-src
	@echo "Created distribution: ../$(NAME).tar.gz"

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJS) $(GUI_OBJS_WITHOUT_MAIN) $(GAME_OBJS) $(TEST_RUNNER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(GUI_OBJS_WITHOUT_MAIN) $(GAME_OBJS) $(TEST_RUNNER) -o $@ $(TEST_LDFLAGS)

$(BIN_DIR)/$(NAME)-test: $(OBJS) $(TEST_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Test executable built: $(BIN_DIR)/$(NAME)-test"

gcov_report:
	@echo "Building with coverage instrumentation..."
	$(MAKE) clean
	$(MAKE) $(TEST_EXEC) CFLAGS="$(CFLAGS) $(GCOV_FLAGS)" LDFLAGS="$(LDFLAGS) $(GCOV_LDFLAGS)" TEST_LDFLAGS="$(TEST_LDFLAGS) $(GCOV_LDFLAGS)"
	
	@echo "Running tests to generate coverage data..."
	./$(TEST_EXEC)
	
	@mkdir -p ../gcov_report
	@echo "Generating coverage report..."
	
	cd .. && gcovr \
		--root . \
		--filter "src/brick_game/tetris" \
		--exclude "src/gui" \
		--exclude "tests" \
		--exclude "src/brick_game/tetris/timer.c" \
		--exclude "src/brick_game/tetris/database.c" \
		--exclude "src/brick_game/tetris/game_process.c" \
		--html-details gcov_report/coverage.html \
		--print-summary
	
	@echo "Coverage report generated: ../gcov_report/coverage.html"

run: clean all
	$(BIN_DIR)/./$(NAME)

clang:
	find . -type f \( -name "*.h" -o -name "*.c" \) -exec clang-format -i {} \;

.PHONY: all install uninstall clean dvi dist test gcov_report clang run