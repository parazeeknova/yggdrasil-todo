#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <leif/leif.h>

typedef enum {
  FILTER_ALL = 0,
  FILTER_IN_PROGRESS,
  FILTER_COMPLETED,
  FILTER_LOW,
  FILTER_MEDIUM,
  FILTER_HIGH
} entry_filter;

typedef enum {
  PRIORITY_LOW = 0,
  PRIORITY_MEDIUM,
  PRIORITY_HIGH
} entry_priority;

typedef struct {
  bool completed;
  char* desc, *date;
  entry_priority priority;
} task_entry;

#define WIN_MARGIN 15.0f

static int winw = 1280, winh = 720;
static LfFont titlefont;
static entry_filter current_filter;

static task_entry* entries[1024];
static uint32_t num_entries = 0;

static LfTexture removetexture;

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

static void renderfilters() {
  uint32_t numfilters = 6;
  static const char* filters[] = {"ALL", "IN PROGRESS", "COMPLETED", "LOW", "MEDIUM", "HIGH"};

  LfUIElementProps props = lf_get_theme().button_props;
  props.color = LF_NO_COLOR;
  props.margin_left = 10.0f;
  props.margin_right = 10.0f;
  props.margin_top = 10.0f;
  props.padding = 10.0f;
  props.border_width = 0.0f;
  props.color = LF_NO_COLOR;
  props.text_color = LF_WHITE;
  props.corner_radius = 8.0f;

  float width = 0.0f;
  float ptrx_before = lf_get_ptr_x();
  float ptry_before = lf_get_ptr_y();
  lf_push_style_props(props);
  lf_set_no_render(true);
  lf_set_ptr_y_absolute(lf_get_ptr_y() + 50.0f);
  for(uint32_t i = 0; i < numfilters; i++) {
    lf_button(filters[i]); 
  }
  lf_set_no_render(false);
  lf_set_ptr_y_absolute(ptry_before);

  width = lf_get_ptr_x() - ptrx_before - props.margin_right - props.padding;

  lf_set_ptr_x_absolute(winw - width - WIN_MARGIN * 2.0f);

  lf_set_line_should_overflow(false);
  for(uint32_t i = 0; i < numfilters; i++) {
    props.color = (current_filter == (entry_filter)i) ? (LfColor){255, 255, 255, 50} : LF_NO_COLOR;
    lf_push_style_props(props);
    if(lf_button(filters[i]) == LF_CLICKED) {
      current_filter = (entry_filter)i;
    }
    lf_pop_style_props();
  }
  lf_set_line_should_overflow(true);
}

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(winw, winh, "Todo", NULL, NULL);

    glfwMakeContextCurrent(window);

    lf_init_glfw(winw, winh, window);

    LfTheme theme = lf_default_theme();
    theme.div_props.color = LF_NO_COLOR;
    lf_set_theme(theme);

    titlefont = lf_load_font("./assets/fonts/inter-bold.ttf", 30);

    removetexture = lf_load_texture("./assets/icons/remove.png", true, LF_TEX_FILTER_LINEAR);

    task_entry* entry = (task_entry*)malloc(sizeof(*entry));
    entry->priority = PRIORITY_LOW;
    entry->completed = false;
    entry->date = "nothin";
    entry->desc = "Buy a Cat";
    entries[num_entries++] = entry;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

        lf_begin();

        lf_div_begin(((vec2s){WIN_MARGIN, WIN_MARGIN}), 
                      ((vec2s){winw - WIN_MARGIN * 2.0f, winh - WIN_MARGIN * 2.0f}),
        true);

        rendertopbar();
        lf_next_line();
        renderfilters();
        lf_next_line();
        {
          lf_div_begin(((vec2s){lf_get_ptr_x(), lf_get_ptr_y()}),
                        ((vec2s){winw - lf_get_ptr_x() - WIN_MARGIN, 
                        (winh - lf_get_ptr_y() - WIN_MARGIN)}),
                        true);
          for(uint32_t i = 0; i < num_entries; i++) {
            task_entry* entry = entries[i];
            float priority_size = 15.0f;
            float ptry_before = lf_get_ptr_y();
            lf_set_ptr_x_absolute(lf_get_ptr_x() + 5.0f);
            lf_set_ptr_y_absolute(lf_get_ptr_y() + 5.0f);
            switch(entry -> priority) {
              case PRIORITY_LOW: {
                lf_rect(priority_size, priority_size, (LfColor){76, 175, 80, 255}, 4.0f);
                break;
              }
              case PRIORITY_MEDIUM: {
                lf_rect(priority_size, priority_size, (LfColor){255, 235, 59, 255}, 4.0f);
                break;
              }
              case PRIORITY_HIGH: {
                lf_rect(priority_size, priority_size, (LfColor){244, 67, 54, 255}, 4.0f);
                break;
              }
            }
            lf_set_ptr_y_absolute(ptry_before);
          
          {
            LfUIElementProps props = lf_get_theme().button_props;
            props.color = LF_NO_COLOR;
            props.border_width = 0.0f; props.padding = 0.0f; props.margin_top = 3.0f; props.margin_left = 10.0f;
            lf_push_style_props(props);
            if(lf_image_button(((LfTexture){.id = removetexture.id, .width = 20, .height = 20})) ==
              LF_CLICKED) {

              }
            lf_pop_style_props();
          }
          {
            LfUIElementProps props = lf_get_theme().checkbox_props;
            props.border_width = 1.0f; props.corner_radius = 0.0f; props.margin_top = 0; props.padding= 5.0f;
            props.margin_left = 5.0f;
            props.color = lf_color_from_zto((vec4s){0.05f, 0.05f, 0.05f, 1.0f});
            lf_push_style_props(props);
            if(lf_checkbox("", &entry->completed, LF_NO_COLOR, ((LfColor){65, 167, 204, 255})) == LF_CLICKED) {
              
            }
            lf_pop_style_props();
          }
            LfUIElementProps props = lf_get_theme().text_props;
            props.margin_top = 4.0;
            props.margin_left = 5.0f;
            lf_push_style_props(props);
            lf_text(entry->desc);
            lf_pop_style_props();
            lf_next_line();
          }
          lf_div_end();
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
