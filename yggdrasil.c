#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <leif/leif.h>

#define WIN_MARGIN 20.0f

static int winw = 1280, winh = 720;
static LfFont titlefont;

static void rendertopbar() {
  lf_push_font(&titlefont);
  lf_text("Yggdrasil - Todo");
  lf_pop_font();

  {
    const float width = 160.0f;

    lf_set_ptr_x_absolute(winw - width - WIN_MARGIN * 2.0f);
    LfUIElementProps props = lf_get_theme().button_props;
    props.margin_left = 0.0f; props.margin_right = 0.0f;
    props.color = (LfColor){65, 167, 204, 255};
    props.border_width = 0.0f; props.corner_radius = 4.0f;
    lf_push_style_props(props);
    lf_set_line_should_overflow(false);
    lf_button_fixed("New Task", 140, -1);
    lf_set_line_should_overflow(true);
    lf_pop_style_props();
  }
}

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(winw, winh, "Todo", NULL, NULL);

    glfwMakeContextCurrent(window);

    lf_init_glfw(winw, winh, window);

    LfTheme theme = lf_default_theme();
    theme.div_props.color = LF_NO_COLOR;
    lf_set_theme(theme);

    titlefont = lf_load_font("./fonts/inter-bold.ttf", 30);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

        lf_begin();

        lf_div_begin(((vec2s){WIN_MARGIN, WIN_MARGIN}), 
                      ((vec2s){winw - WIN_MARGIN * 2.0f, winh - WIN_MARGIN * 2.0f}),
        true);

        rendertopbar();

        lf_next_line();

        {
          uint32_t numfilters = 6;
          static const char* filters[] = {"ALL", "IN PROGRESS", "COMPLETED", "LOW", "MEDIUM", "HIGH"};

          LfUIElementProps props = lf_get_theme().text_props;
          props.margin_top = 15.0f;
          props.margin_right = 25.0f;

          float width = 0.0f;
          float ptrx_before = lf_get_ptr_x();
          lf_push_style_props(props);
          lf_set_no_render(true);
          for(uint32_t i = 0; i < numfilters; i++) {
            lf_button(filters[i]); 
          }
          lf_set_no_render(false);

          width = lf_get_ptr_x() - ptrx_before - props.margin_right - props.padding;

          lf_set_ptr_x_absolute(winw - width - WIN_MARGIN * 2.0f);

          lf_set_line_should_overflow(false);
          for(uint32_t i = 0; i < numfilters; i++) {
            lf_push_style_props(props);
            lf_button(filters[i]);
            lf_pop_style_props();
          }
          lf_set_line_should_overflow(true);
        }

        lf_div_end();
        lf_end();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    lf_free_font(&titlefont);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
