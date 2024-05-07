#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

struct Img {
    Texture texture;
    char *filename;
};

#define BG_COLOR (Color){35, 35, 35, 255}

static const int screen_width = 800;
static const int screen_height = 600;

int main(int argc, char **argv) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: gucker <images>\n");
        return 1;
    }
    InitWindow(800, 600, "gucker");
    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.zoom = 1.f;

    int images_len = argc-1;
    struct Img images[images_len];
    for (int i = 0; i < images_len; ++i) {
        Image img = LoadImage(argv[i+1]);
        if (img.data == NULL) return 1;
        images[i].texture = LoadTextureFromImage(img);
        images[i].filename = argv[i+1];
        UnloadImage(img);
    }

    int current_image = 0;
    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;

            const float zoomIncrement = 0.125f;
            camera.zoom += (wheel*zoomIncrement);
            if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
        }

        const int rect_size = 30;
        Rectangle prev = {
            .x = screen_width/2.f-rect_size*2,
            .y = screen_height-rect_size-10,
            .width = rect_size,
            .height = rect_size
        };
        Rectangle next = {
            .x = screen_width/2.f+rect_size,
            .y = screen_height-rect_size-10,
            .width = rect_size,
            .height = rect_size
        };
        if (images_len > 1) {
            int set = 0;
            if (CheckCollisionPointRec(GetMousePosition(), prev)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && current_image > 0) {
                    current_image--;
                }
                set = 1;
            }
            if (CheckCollisionPointRec(GetMousePosition(), next)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && current_image < images_len-1) {
                    current_image++;
                }
                set = 1;
            }
            if (!set) {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
        }
        Texture texture = images[current_image].texture;
        
        BeginDrawing();
        ClearBackground(BG_COLOR);

        BeginMode2D(camera);
        DrawTexture(texture, screen_width/2 - texture.width/2, screen_height/2 - texture.height/2 - 40, WHITE);
        EndMode2D();

        int text_y = screen_height-40;
        if (images_len > 1) {
            DrawRectangleRec(prev, BLACK);
            DrawRectangleRec(next, BLACK);
            DrawLineEx((Vector2){screen_width/2.f-rect_size*2+5, screen_height-rect_size/2.f-10}, (Vector2){screen_width/2.f-rect_size-5, screen_height-rect_size-5}, 2.f, RAYWHITE);
            DrawLineEx((Vector2){screen_width/2.f-rect_size*2+5, screen_height-rect_size/2.f-10}, (Vector2){screen_width/2.f-rect_size-5, screen_height-15}, 2.f, RAYWHITE);
            DrawLineEx((Vector2){screen_width/2.f+rect_size+5, screen_height-rect_size-5}, (Vector2){screen_width/2.f+rect_size*2-5, screen_height-rect_size/2.f-10}, 2.f, RAYWHITE);
            DrawLineEx((Vector2){screen_width/2.f+rect_size+5, screen_height-15}, (Vector2){screen_width/2.f+rect_size*2-5, screen_height-rect_size/2.f-10}, 2.f, RAYWHITE);
            text_y = screen_height-rect_size-40;
        }
        DrawText(images[current_image].filename, screen_width/2.f-(MeasureText(images[current_image].filename, 22)/2.f), text_y, 22, RAYWHITE);
        EndDrawing();
    }
    
    for (int i = 0; i < images_len; ++i) {
        UnloadTexture(images[i].texture);
    }
    CloseWindow();
    return 0;
}
