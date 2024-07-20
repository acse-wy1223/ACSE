import os
import logging
import graphviz
import matplotlib.pyplot as plt
import pandas as pd
from PIL import Image, ImageDraw, ImageFont

# Define the output directories and file names
OUTPUT_DIR = "./output"
POST_DOC_DIR = "./post-proc"
VECTOR_FILE = "vector.dat"
PERFORMANCE_FILE = "performance.dat"
LOG_INFO = logging.CRITICAL

# Configure logging
logging.basicConfig(
    level=LOG_INFO,
    format="%(asctime)s - %(levelname)s - %(message)s - %(name)s - %(lineno)d",
)


def read_data(
    vector_file: str = "", performance_file: str = ""
) -> tuple[pd.DataFrame, pd.DataFrame]:
    """
    Reads the circuit configuration and monetary value data
    from the specified files.
    """
    try:
        logging.info(f"Reading data from {vector_file} and {performance_file}") # noqa
        circuit_conf: pd.DataFrame = pd.read_csv(vector_file, sep=" ",
                                                 header=None)
        performance_values: pd.DataFrame = pd.read_csv(
            performance_file, sep=" ", header=None
        )
        circuit_conf = circuit_conf.dropna(axis=1, how="all")
        performance_values = performance_values.dropna(axis=1, how="all")
        return (circuit_conf.values.reshape(-1),
                performance_values.values.reshape(-1))
    except FileNotFoundError as e:
        logging.error(f"File not found: {e}")
    except Exception as e:
        logging.error(f"An error occurred while reading the files: {e}")
    return None, None


def calculate_font_size(
    cell_width, cell_height, text, max_font_size=20, padding=(0, 0)
):
    """
    Calculate the appropriate font size to fit the text within
    the given cell dimensions.
    """
    font_size = max_font_size
    while font_size > 0:
        try:
            font = ImageFont.truetype("Arial.ttf", font_size)
        except IOError:
            try:
                font = ImageFont.truetype("/Library/Fonts/Arial.ttf",
                                          font_size)
            except IOError:
                font = ImageFont.load_default()
                logging.warning("Arial font not found. Using default font.") # noqa
        _, _, text_width, text_height = font.getbbox(text)
        if (
            text_width <= cell_width - padding[0]
            and text_height <= cell_height - padding[1]
        ):
            return font_size
        font_size -= 1
    return font_size


def create_table_image(
    vector_array, start_x=0, start_y=0, image_size=(600, 400),
    table_size=(300, 200)
):
    """
    Creates an image with a table displaying the vector array
    in the specified format.
    """
    try:
        image = Image.new("RGB", image_size, "white")
        draw = ImageDraw.Draw(image)

        total_width = table_size[0]
        header_cell_width = total_width * 1.3 // len(vector_array) * 3
        number_cell_width = total_width * 1.3 // len(vector_array)
        cell_height = 40

        header_font_size = calculate_font_size(
            header_cell_width, cell_height, "Unit XXX", padding=(10, 1)
        )
        number_font_size = calculate_font_size(number_cell_width,
                                               cell_height, "00")

        try:
            header_font = ImageFont.truetype("Arial.ttf",
                                             int(header_font_size // 1.2))
            number_font = ImageFont.truetype("Arial.ttf",
                                             int(number_font_size // 1.2))
        except IOError:
            try:
                header_font = ImageFont.truetype(
                    "/Library/Fonts/Arial.ttf", header_font_size
                )
                number_font = ImageFont.truetype(
                    "/Library/Fonts/Arial.ttf", number_font_size
                )
            except IOError:
                header_font = ImageFont.load_default()
                number_font = ImageFont.load_default()
                logging.warning("Arial font not found. Using default font.") # noqa

        headers = ["Feed"]
        for i in range(1, len(vector_array), 3):
            headers.append(f"Unit {i // 3}")

        current_x = start_x
        draw.rectangle(
            [current_x, start_y, current_x + number_cell_width,
             start_y + cell_height],
            outline="black",
        )
        feed_text_width = draw.textlength("Feed", font=header_font)
        feed_text_height = feed_text_width // 3
        draw.text(
            (
                current_x + (number_cell_width - feed_text_width) // 2,
                start_y + (cell_height - feed_text_height // 2) // 2,
            ),
            "Feed",
            font=header_font,
            fill="black",
        )
        current_x += number_cell_width

        header = max(headers, key=len)
        header_text_width = draw.textlength(header, font=header_font)
        header_text_height = header_text_width // 3
        # Draw the other headers
        for header in headers[1:]:
            draw.rectangle(
                [
                    current_x,
                    start_y,
                    current_x + header_cell_width,
                    start_y + cell_height,
                ],
                outline="black",
            )
            draw.text(
                (
                    current_x + (header_cell_width - header_text_width) // 2,  # noqa
                    start_y + (cell_height - header_text_height // 2) // 2,
                ),
                header,
                font=header_font,
                fill="black",
            )
            current_x += header_cell_width

        current_x = start_x
        current_y = start_y + cell_height

        value = str(max(vector_array))
        value_text_width = draw.textlength(value, font=number_font)
        value_text_height = value_text_width // 3

        # Draw the "Feed" value
        draw.rectangle(
            [
                current_x,
                current_y,
                current_x + number_cell_width,
                current_y + cell_height,
            ],
            outline="black",
        )
        feed_value_text_width = value_text_width
        feed_value_text_height = value_text_height

        draw.text(
            (
                current_x + (number_cell_width - feed_value_text_width) // 2,  # noqa
                current_y + (cell_height - feed_value_text_height // 2) // 2,
            ),
            str(vector_array[0]),
            font=number_font,
            fill="black",
        )
        current_x += number_cell_width

        for i in range(1, len(vector_array)):
            if (i - 1) % 3 == 2:
                current_x += header_cell_width - 3 * number_cell_width
            draw.rectangle(
                [
                    current_x,
                    current_y,
                    current_x + number_cell_width,
                    current_y + cell_height,
                ],  # noqa
                outline="black",
            )
            draw.text(
                (
                    current_x + (number_cell_width - value_text_width) // 2,
                    current_y + (cell_height - value_text_height // 2) // 2,
                ),
                str(vector_array[i]),
                font=number_font,
                fill="black",
            )
            current_x += number_cell_width

        return image
    except Exception as e:
        logging.error(f"An error occurred while creating the table image: {e}")


def create_performance_table_image(
    performance_values,
    headers,
    start_x=0,
    start_y=0,
    image_size=(600, 400),
    table_size=(300, 200),
):
    """
    Creates an image with a table displaying the monetary value in the specified format. # noqa

    Parameters:
    performance_values (list): The monetary value to be displayed in the table.
    headers (list): The list of headers for the table.
    image_size (tuple): The size of the image (width, height).
    table_size (tuple): The size of the table (width, height).
    """
    if len(headers) != len(performance_values):
        logging.error("Headers length and performance values length must be equal.") # noqa
        return None

    try:
        image = Image.new("RGB", image_size, "white")
        draw = ImageDraw.Draw(image)

        total_width = table_size[0]
        header_cell_width = total_width // len(headers)
        number_cell_width = total_width // len(performance_values)
        cell_height = 40

        header_font_size = calculate_font_size(
            header_cell_width, cell_height, "Optimum", padding=(0, 0)
        )
        number_font_size = calculate_font_size(number_cell_width, cell_height, "00") # noqa

        try:
            header_font = ImageFont.truetype("Arial.ttf", int(header_font_size // 1.2)) # noqa
            number_font = ImageFont.truetype("Arial.ttf", int(number_font_size // 1.2)) # noqa
        except IOError:
            try:
                header_font = ImageFont.truetype("/Library/Fonts/Arial.ttf", header_font_size) # noqa
                number_font = ImageFont.truetype("/Library/Fonts/Arial.ttf", number_font_size) # noqa
            except IOError:
                header_font = ImageFont.load_default()
                number_font = ImageFont.load_default()
                logging.warning("Arial font not found. Using default font.") # noqa

        current_x = start_x
        current_y = start_y

        for header, value in zip(headers, performance_values):
            # Draw header
            draw.rectangle(
                [
                    current_x,
                    current_y,
                    current_x + header_cell_width,
                    current_y + cell_height,
                ],
                outline="black",
            )
            header_text_width = draw.textlength(header, font=header_font)
            header_text_height = header_text_width // 3
            draw.text(
                (
                    current_x + (header_cell_width // 2 - header_text_width // 2), # noqa
                    current_y + (cell_height - header_text_height // 2) // 2,
                ),
                header,
                font=header_font,
                fill="black",
            )

            # Draw value
            draw.rectangle(
                [
                    current_x,
                    current_y + cell_height,
                    current_x + number_cell_width,
                    current_y + 2 * cell_height,
                ],
                outline="black",
            )
            value_text_width = draw.textlength(str(value), font=number_font)
            value_text_height = value_text_width // 3
            if value < 1:
                value = f"{(value*100):.2f}%"
            else:
                value = f"{value:.2f}"
            draw.text(
                (
                    current_x + (number_cell_width - value_text_width) // 2,
                    current_y + cell_height + (cell_height - value_text_height) // 2, # noqa
                ),
                str(value),
                font=number_font,
                fill="black",
            )

            current_x += header_cell_width

        return image
    except Exception as e:
        logging.error(f"An error occurred while creating the table image: {e}")


def create_diagram(array: pd.DataFrame, show_labels=True) -> str:
    logging.info("Creating diagram")
    try:
        num_units = (len(array) - 1) // 3
        dot = graphviz.Digraph(comment="Circuit Graph",
                               graph_attr={"rankdir": "LR"})

        color_map = {"Feed": "black", "Conc": "blue",
                     "Inter": "purple", "Tails": "red"}

        dot.node("Feed", "Feed", color=color_map["Feed"], shape="box")
        for i in range(num_units):
            dot.node(f"Unit {i}", f"Unit {i}", shape="box")
        dot.node("Concentrate", "Concentrate",
                 color=color_map["Conc"], shape="box")
        dot.node("Tailings", "Tailings",
                 color=color_map["Tails"], shape="box")

        dot.edge(
            "Feed",
            f"Unit {array[0]}",
            label="Feed" if show_labels else "",
            color=color_map["Feed"],
        )

        for i in range(num_units):
            conc_dest = array[3 * i + 1]
            inter_dest = array[3 * i + 2]
            tails_dest = array[3 * i + 3]

            if conc_dest < num_units:
                dot.edge(
                    f"Unit {i}",
                    f"Unit {conc_dest}",
                    label="Conc" if show_labels else "",
                    color=color_map["Conc"],
                    headport="w",
                    tailport="n",
                )
            elif conc_dest == num_units:
                dot.edge(
                    f"Unit {i}",
                    "Concentrate",
                    label="Conc" if show_labels else "",
                    color=color_map["Conc"],
                    headport="w",
                    tailport="n",
                )

            if inter_dest < num_units:
                dot.edge(
                    f"Unit {i}",
                    f"Unit {inter_dest}",
                    label="Inter" if show_labels else "",
                    color=color_map["Inter"],
                    headport="w",
                    tailport="e",
                )
            elif inter_dest == num_units:
                dot.edge(
                    f"Unit {i}",
                    "Concentrate",
                    label="Inter" if show_labels else "",
                    color=color_map["Inter"],
                    headport="w",
                    tailport="e",
                )

            if tails_dest < num_units:
                dot.edge(
                    f"Unit {i}",
                    f"Unit {tails_dest}",
                    label="Tails" if show_labels else "",
                    color=color_map["Tails"],
                    headport="w",
                    tailport="s",
                )
            elif tails_dest == num_units + 1:
                dot.edge(
                    f"Unit {i}",
                    "Tailings",
                    label="Tails" if show_labels else "",
                    color=color_map["Tails"],
                    headport="w",
                    tailport="s",
                )

        file_path = f"{POST_DOC_DIR}/flowchart"
        file = dot.render(filename=file_path, format="png", cleanup=True)
        if LOG_INFO:
            logging.info(f"Diagram created and saved to {file}")
        return file
    except Exception as e:
        logging.error(f"An error occurred while creating the diagram: {e}")
        return ""


def get_concat_v(im1, im2):
    dst = Image.new("RGB", (im1.width, im1.height + im2.height))
    dst.paste(im1, (0, 0))
    dst.paste(im2, (0, im1.height))
    return dst


def annotate_image(file: str, circuit_conf, performance_values,
                   headers=["Performance", "Recovery", "Grade"]) -> str:
    """
    Annotates the diagram image with the circuit configuration vector and performance values. # noqa

    Parameters:
    file (str): The file path of the diagram image.
    circuit_conf: The circuit configuration data.
    performance_values: The monetary value data.
    headers: The list of headers for the table.

    Returns:
    str: The file path of the annotated diagram image.
    """
    logging.info("Annotating image")
    try:
        image = Image.open(file)
        table = create_table_image(
            circuit_conf,
            start_x=image.width // 7,
            start_y=20,
            image_size=(image.width, 150),
            table_size=(4 * image.width // 7, 150),
        )
        image = get_concat_v(image, table)
        stats = create_performance_table_image(
            performance_values,
            start_x=image.width // 5,
            start_y=0,
            headers=headers,
            image_size=(image.width, 100),
            table_size=(3 * image.width // 5, 100),
        )
        image = get_concat_v(image, stats)
        image.save(file)
        if LOG_INFO:
            logging.info(f"Image annotated and saved to {file}")
        return file
    except Exception as e:
        logging.error(f"An error occurred while annotating the image: {e}")
        return ""


def display_image(file: str) -> None:
    """
    Displays the annotated diagram image using matplotlib.
    """
    if LOG_INFO:
        logging.info(f"Displaying image {file}")
    try:
        image = plt.imread(file)
        plt.imshow(image)
        plt.axis("off")
        plt.show()
    except Exception as e:
        logging.error(f"An error occurred while displaying the image: {e}")


def main():
    """
    Main function to orchestrate reading data, creating diagram, annotating image, and displaying image.
    """  # noqa
    # Read the circuit configuration and performance values
    circuit_conf, performance_values = read_data(
        os.path.join(OUTPUT_DIR, VECTOR_FILE),
        os.path.join(OUTPUT_DIR, PERFORMANCE_FILE),
    )
    # Make sure the performance values are in the correct format
    if len(performance_values) != 3:
        headers = ["Performance"]
        for i in range(len(performance_values) - 1):
            headers.append(f"Value {i+1}")
    else:
        headers = ["Performance", "Recovery", "Grade"]
    # Create the diagram and annotate the image
    if circuit_conf is not None and performance_values is not None:
        file = create_diagram(circuit_conf, show_labels=False)
        if file:
            annotated_file = annotate_image(file, circuit_conf,
                                            performance_values, headers=headers) # noqa
            if annotated_file:
                display_image(annotated_file)


if __name__ == "__main__":
    main()
