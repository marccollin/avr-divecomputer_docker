import os

# Semihosting trace output (trace_printf) halts the CPU when a debug probe has
# enabled debug mode but no debugger is servicing it (e.g. right after flashing),
# so it is only built in with: scons TRACE=1 run|attach
use_trace = ARGUMENTS.get('TRACE', '0') == '1'

toolchain_prefix = ARGUMENTS.get('TOOLCHAIN', '')
if toolchain_prefix and not toolchain_prefix.endswith('/'):
    toolchain_prefix += '/'

env = Environment(
    ENV=os.environ,
    CC=toolchain_prefix + 'arm-none-eabi-gcc',
    CXX=toolchain_prefix + 'arm-none-eabi-g++',
    RANLIB=toolchain_prefix + 'arm-none-eabi-ranlib',
    AR=toolchain_prefix + 'arm-none-eabi-ar',
    OBJCOPY=toolchain_prefix + 'arm-none-eabi-objcopy',
    CCLDFLAGS='-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og',
    CONLYFLAGS='-std=gnu11',
    CANDCXXFLAGS='$CCLDFLAGS -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -fabi-version=0 -fno-exceptions -Wabi',
    LINKFLAGS='$CCLDFLAGS -T mem.ld -T libs.ld -T sections.ld -nostartfiles --specs=nano.specs -Wl,--gc-sections -u _printf_float',
    CFLAGS='$CANDCXXFLAGS $CONLYFLAGS',
    CXXFLAGS='$CANDCXXFLAGS -Wnoexcept -Wctor-dtor-privacy  -Wsign-promo -fno-threadsafe-statics -Wnon-virtual-dtor -fno-rtti -Wstrict-null-sentinel -fno-use-cxa-atexit -std=gnu++11',
    CPPPATH=[
        'include',
        'system/include/diag',
        'system/include',
        'system/include/cmsis',
        'system/include/stm32f4-hal',
        'lib/u8glib/csrc',
        'lib/fatfs/src',
    ],
    CPPDEFINES= {
        'DEBUG': None,
        'USE_FULL_ASSERT': None,
        'STM32F407xx': None,
        'USE_HAL_DRIVER': None,
        'HSE_VALUE': '8000000',
    },
    LIBPATH=['.', '#system/ldscripts', '#system/led-test/ldscripts'],
    OPENOCD_FLAGS='-f board/stm32f4discovery.cfg -c init'
)
if use_trace:
    env.Append(CPPDEFINES={'OS_USE_TRACE_SEMIHOSTING_STDOUT': None, 'TRACE': None})


env.Library('runtime',
    Glob('system/src/cortexm/*.c') +
    Glob('system/src/diag/*.c') +
    Glob('system/src/newlib/*.cpp') +
    Glob('system/src/stm32f4-hal/*.c') +
    Glob('lib/fatfs/src/*.c') +
    Glob('lib/fatfs/src/option/unicode.c')
)

env.Library('runtimenogc',
    # The following system files are not tolerant to linker GC.
    Glob('system/src/cmsis/*.c') +
    Glob('system/src/newlib/*.c')
)

u8env = env.Clone()
u8env.Append(CANDCXXFLAGS=' -Wno-unused-parameter -Wno-missing-declarations -Wno-unused-variable -Wno-unused-function')
u8env.Library('u8glib', Glob('lib/u8glib/csrc/*') + Glob('lib/u8glib/sfntsrc/*.c'))

Default(env.Command('DiveComputer.hex', 'DiveComputer', '$OBJCOPY -O ihex $SOURCES $TARGET'))
upload = env.Command('upload_phony', 'DiveComputer.hex', 'openocd $OPENOCD_FLAGS -c "reset halt" -c "flash write_image erase ${SOURCES}" -c "verify_image ${SOURCES}" -c "reset run" -c shutdown')
env.Alias('upload', 'upload_phony')

run = env.Command('run_phony', 'DiveComputer.hex', 'openocd $OPENOCD_FLAGS -c "reset halt" -c "arm semihosting enable" -c "reset run"')
env.Alias('run', 'run_phony')

attach = env.Command('attach_phony', 'DiveComputer.hex', 'openocd $OPENOCD_FLAGS -c "reset halt" -c "arm semihosting enable"')
env.Alias('attach', 'attach_phony')

# Attach without resetting, report where the CPU is, then let it continue (diagnose hangs).
def where_action(target, source, env):
    import subprocess
    nm = subprocess.check_output([toolchain_prefix + 'arm-none-eabi-nm', str(source[0])]).decode()
    tick = next((l.split()[0] for l in nm.splitlines() if l.endswith(' uwTick')), None)
    cmds = ['halt', 'reg pc', 'reg lr', 'reg sp', 'reg xPSR',
            'mdw 0xE000EDF0',          # DHCSR
            'mdw 0xE000E010 3',        # SysTick CSR / RVR / CVR
            'mdw 0xE000ED04',          # ICSR (pending / active exception)
            'mdw [lindex [reg sp] 2] 16']         # stack, to find the caller
    if tick:
        cmds += ['mdw 0x' + tick, 'resume', 'sleep 500', 'halt', 'mdw 0x' + tick]
    cmds += ['resume', 'shutdown']
    args = ['openocd'] + env['OPENOCD_FLAGS'].split()
    for c in cmds:
        args += ['-c', c]
    return subprocess.call(args)

where = env.Command('where_phony', 'DiveComputer', where_action)
env.Alias('where', 'where_phony')
AlwaysBuild(where)

SideEffect('openocd', upload + run + attach + where)

env.Program('DiveComputer', Glob('src/*.c') + Glob('src/*.cpp'), LIBS=['runtime', 'runtimenogc', 'u8glib', 'm'], LINKFLAGS='$LINKFLAGS -Wl,--whole-archive -lruntimenogc -Wl,--no-whole-archive')
