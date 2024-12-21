#include "gameConfig.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Point* parse_piece_points(json_object* piece_array, int* size) {
    *size = json_object_array_length(piece_array);
    Point* points = (Point*)malloc(sizeof(Point) * (*size));
    
    for (int i = 0; i < *size; i++) {
        json_object* point_obj = json_object_array_get_idx(piece_array, i);
        json_object* x_obj, *y_obj;
        
        json_object_object_get_ex(point_obj, "x", &x_obj);
        json_object_object_get_ex(point_obj, "y", &y_obj);
        
        points[i].x = json_object_get_int(x_obj);
        points[i].y = json_object_get_int(y_obj);
    }
    
    return points;
}

GameConfig* load_config_from_file(const char* filename) {
    GameConfig* config = (GameConfig*)malloc(sizeof(GameConfig));
    json_object *root_obj, *colors_array, *pieces_array, *velocity_down, *velocity_movement;
    
    // Read JSON file
    root_obj = json_object_from_file(filename);
    if (!root_obj) {
        fprintf(stderr, "Error reading config file: %s\n", filename);
        free(config);
        return NULL;
    }
    
    // Parse colors
    json_object_object_get_ex(root_obj, "colors", &colors_array);
    config->colorsCount = json_object_array_length(colors_array);
    config->colors = (ColorConfig*)malloc(sizeof(ColorConfig) * config->colorsCount);
    
    for (int i = 0; i < config->colorsCount; i++) {
        json_object* color_obj = json_object_array_get_idx(colors_array, i);
        json_object *key_obj, *class_obj, *score_obj;
        
        json_object_object_get_ex(color_obj, "key", &key_obj);
        GridValue enumValue = (GridValue)json_object_get_int(key_obj);
        json_object_object_get_ex(color_obj, "className", &class_obj);
        json_object_object_get_ex(color_obj, "scoreValue", &score_obj);
        
        config->colors[i].key = enumValue;
        config->colors[i].className = strdup(json_object_get_string(class_obj));
        config->colors[i].scoreValue = json_object_get_int(score_obj);
    }
    
    // Parse pieces
    json_object_object_get_ex(root_obj, "pieces", &pieces_array);
    config->piecesCount = json_object_array_length(pieces_array);
    config->pieces = (Point**)malloc(sizeof(Point*) * config->piecesCount);
    config->pieceSizes = (int*)malloc(sizeof(int) * config->piecesCount);
    
    for (int i = 0; i < config->piecesCount; i++) {
        json_object* piece_array = json_object_array_get_idx(pieces_array, i);
        config->pieces[i] = parse_piece_points(piece_array, &config->pieceSizes[i]);
    }
    
    // Parse velocities
    json_object_object_get_ex(root_obj, "velocityDown", &velocity_down);
    json_object_object_get_ex(root_obj, "velocityMovement", &velocity_movement);
    
    config->velocityDown = json_object_get_int(velocity_down);
    config->velocityMovement = json_object_get_int(velocity_movement);
    
    // Clean up JSON object
    json_object_put(root_obj);
    
    return config;
}

void destroy_game_config(GameConfig* config) {
    if (config == NULL) return;
    
    // Free colors
    for (int i = 0; i < config->colorsCount; i++) {
        free((char*)config->colors[i].className);
    }
    free(config->colors);
    
    // Free pieces
    for (int i = 0; i < config->piecesCount; i++) {
        free(config->pieces[i]);
    }
    free(config->pieces);
    free(config->pieceSizes);
    
    // Free the config struct itself
    free(config);
} 