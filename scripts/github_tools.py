import os
import sys


class GitHubLogger:
    @staticmethod
    def info(message, file=None, line=None, endLine=None, title=None):
        GitHubLogger._print_formatted("notice", message, file, endLine, title)

    @staticmethod
    def warn(message, file=None, line=None, endLine=None, title=None):
        GitHubLogger._print_formatted("warning", message, file, endLine, title)

    @staticmethod
    def error(message, file=None, line=None, endLine=None, title=None):
        GitHubLogger._print_formatted("error", message, file, endLine, title)

    @staticmethod
    def print(*args, **kwargs):
        if "GITHUB_ACTIONS" in os.environ:
            print(*args, **kwargs)
            sys.stdout.flush()

    @staticmethod
    def _print_formatted(
        output_type, message, file=None, line=None, endLine=None, title=None
    ):
        GitHubLogger.print(
            f"::{output_type} {GitHubLogger._format_args(file, line, endLine, title)}::{message}"
        )

    @staticmethod
    def _format_args(file=None, line=None, endLine=None, title=None):
        args = {"file": file, "line": line, "endLine": endLine, "title": title}
        return ",".join(f"{arg}={args[arg]}" for arg in args if args[arg] is not None)
