from rhubarb.texture import Texture
from OpenGL.GL import *
from glfw.GLFW import *
import numpy as np
import sys
import os
import glob
import __main__

from PIL import Image

class Window:
    """creates a glfw window to display textures"""

    VERT_SOURCE = """
#version 330 core 

layout (location = 0) in vec2 vert_pos;
layout (location = 1) in vec2 tex_pos;

out vec2 uv;

void main() {
    gl_Position = vec4(vert_pos, 0, 1.0);
    uv = tex_pos;
}
    """

    FRAG_SOURCE = """
#version 330 core

uniform sampler2D tex;

in vec2 uv;

void main() {
    gl_FragColor = texture(tex, uv);
}
    """

    def __init__(self, size: tuple[int, int], 
                       pos:  tuple[int, int]) -> None:

        if not glfwInit(): sys.exit("glfw did not initialize!")

        file = __main__.__file__
        self.name = file[file.rfind("/")+1:file.rfind(".py")]

        # create window
        self.window = glfwCreateWindow(*size, self.name, None, None)
        self.size = size

        # vars used to store and calculate mspf
        self.mspf = 0.0
        self.last_time = glfwGetTime()

        self.image_path = "images/" + self.name
        self.snapshot_path = "snapshots"

        self.snapshot_count = len(glob.glob(
            "%s/%s*"
            % (self.snapshot_path, self.name)))

        self.snapshot_saved = False

        if not os.path.exists(self.image_path):
            os.makedirs(self.image_path)

        if not os.path.exists(self.snapshot_path):
            os.makedirs(self.snapshot_path)
        
        glfwSetWindowPos(self.window, *pos)
        glfwMakeContextCurrent(self.window)

        def cbfun(window, width, height):
            glfwSetWindowAspectRatio(window, *size)
            glViewport(0, 0, width, height)

        glfwSetFramebufferSizeCallback(self.window, cbfun)

        # setup vertex buffer
        vertices = (
            -1.0, -1.0, 0.0, 1.0,
            -1.0,  1.0, 0.0, 0.0,
             1.0,  1.0, 1.0, 0.0,
            -1.0, -1.0, 0.0, 1.0,
             1.0, -1.0, 1.0, 1.0,
             1.0,  1.0, 1.0, 0.0
        )

        vertices = np.array(vertices, dtype = np.float32)

        vbo = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, vbo)
        glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_STATIC_DRAW)

        glEnableVertexAttribArray(0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertices.itemsize * 4, ctypes.c_void_p(0))

        glEnableVertexAttribArray(1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertices.itemsize * 4, ctypes.c_void_p(8))

        # create vertex and fragment shader for displaying render target
        self.program = glCreateProgram()

        vert_shader = glCreateShader(GL_VERTEX_SHADER)
        glShaderSource(vert_shader, Window.VERT_SOURCE)
        glCompileShader(vert_shader)
        glAttachShader(self.program, vert_shader)
        glDeleteShader(vert_shader)

        frag_shader = glCreateShader(GL_FRAGMENT_SHADER)
        glShaderSource(frag_shader, Window.FRAG_SOURCE)
        glCompileShader(frag_shader)
        glAttachShader(self.program, frag_shader)
        glDeleteShader(frag_shader)
        
        glBindAttribLocation(self.program, 0, "vPosition")
        glLinkProgram(self.program)


    def __del__(self) -> None:
        """terminates glfw library"""
        glfwTerminate()

    def set_render_target(self, target: Texture) -> None:
        glUseProgram(self.program)
        glUniform1i(glGetUniformLocation(
            self.program, "tex"), target.unit)

        def cbfun(window, key, scancode, action, mods):
            if key == GLFW_KEY_S and action == GLFW_PRESS:
                image_count = len(glob.glob(
                    "%s/%s%i.*"
                    % (self.image_path, self.name, 
                       self.snapshot_count)))

                if not self.snapshot_saved:
                    snapshot = open(
                        "%s/%s%i.py" 
                        % (self.snapshot_path, self.name, 
                           self.snapshot_count),
                        "x")
                    snapshot.write(open(__main__.__file__).read())
                    snapshot.close()

                    self.snapshot_saved = True

                target.save(
                    "%s/%s%i.%i.png"
                    % (self.image_path, self.name, 
                       self.snapshot_count, image_count))

        glfwSetKeyCallback(self.window, cbfun)
        
    def should_close(self) -> bool:
        """returns if glfw window should close"""
        return glfwWindowShouldClose(self.window)

    def draw(self) -> None:
        glUseProgram(self.program)
        glDrawArrays(GL_TRIANGLES, 0, 6)

    def update(self) -> None:
        """swaps window's front and back buffers"""
        glfwSwapBuffers(self.window)
        glfwPollEvents()

        # measure mspf
        current_time = glfwGetTime()
        self.mspf = round((current_time - self.last_time) * 1000, 2)
        self.last_time = current_time

    def set_aspect_ratio(self, ratio: tuple):
        glfwSetWindowAspectRatio(self.window, *ratio)
