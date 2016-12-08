defmodule PhoenixChat.RoomChannel do
  use Phoenix.Channel
  require Logger

  def join("rooms:lobby", message, socket) do
    Logger.info("joined")
    {:ok, socket}
  end
end