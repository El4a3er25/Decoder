import os
import lit.formats

config.name = 'Decoder LIT Tests'
config.test_format = lit.formats.ShTest()
config.suffixes = ['.test']
config.test_source_root = os.path.dirname(__file__)

# Define the %decoder substitution to point to our executable
config.substitutions.append(('%decoder', os.path.join(os.path.dirname(__file__), '..', 'decoder')))
